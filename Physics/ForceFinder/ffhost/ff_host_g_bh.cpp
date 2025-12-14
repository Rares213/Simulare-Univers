#include "ff_host_g_bh.hpp"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>

#include "Timer.hpp"

namespace forcefinder
{
	namespace ffh
	{
		template<typename T>
		ffh::ForceFinderGravityBH<T>::ForceFinderGravityBH(BarnesHutGravSpec<T>& init_struct)
		:
		AForceFinderGravity<T>(init_struct.gravity),
		m_theta(init_struct.theta * init_struct.theta)
		{
			_findMostDistantCoord();

			std::vector<WorkerData> workers_data = commfn::getWorkersData(this->m_count, this->m_workers_count);

			for (size_t i = 0; i < workers_data.size(); i++)
			{
				std::function<void(void)> worker_fun = std::bind(&ForceFinderGravityBH<T>::_findForceParallel, this, workers_data[i].start, workers_data[i].end);
				this->m_task_manager.add_task(i, std::move(worker_fun), true);
			}
		}

		template<typename T>
		void forcefinder::ffh::ForceFinderGravityBH<T>::findForce()
		{
			timer::Timer timerff;
			
			_createRoot();
			_buildTree();
			_findCenterOfMass();

			if(this->m_workers_count == 0)
				_findForce();
			else
			{
				for (size_t i = 0; i < this->m_workers_count; i++)
					this->m_task_manager.launch_task(i);

				this->m_task_manager.wait_all_finished();
			}

			_deleteTree(m_root);

			m_root = nullptr;
			this->m_duration = timerff.getDuration();
		}

		template<typename T>
		std::vector<T>::iterator ForceFinderGravityBH<T>::_maxElemVec(std::vector<T>& vec)
		{
			return std::max_element(vec.begin(), vec.end(), 
				[](const auto& largest, const auto& first)
				{
					if (std::abs(first) > std::abs(largest))
						return true;
					else
						return false;
				});
		}

		template<typename T>
		void ffh::ForceFinderGravityBH<T>::_findMostDistantCoord()
		{
			auto x_largest = std::abs(*_maxElemVec(this->m_gravity.position->operator[](0)));
			auto y_largest = std::abs(*_maxElemVec(this->m_gravity.position->operator[](1)));
			auto z_largest = std::abs(*_maxElemVec(this->m_gravity.position->operator[](2)));

			float largest = std::max(std::max(x_largest, y_largest), z_largest);

			m_largest_coord = 3 * largest;
		}

		template<typename T>
		void ForceFinderGravityBH<T>::_createRoot()
		{
			m_root = new Node();

			m_root->cube.half_len = m_largest_coord;
		}

		template<typename T>
		void ForceFinderGravityBH<T>::_buildTree()
		{
			for (size_t i = 0; i < this->m_gravity.position->operator[](0).size(); i++)
				if(_checkBodyInCube(i))
					_addBody(i, m_root);
		}

		template<typename T>
		void ForceFinderGravityBH<T>::_findCenterOfMass()
		{
			for (size_t i = 0; i < this->m_gravity.position->operator[](0).size(); i++)
				if (_checkBodyInCube(i))
					_contributeCenterOfMass(i, m_root);
		}

		template<typename T>
		void ForceFinderGravityBH<T>::_findForce()
		{
			for (size_t i = 0; i < this->m_gravity.position->operator[](0).size(); i++)
				if (_checkBodyInCube(i))
					_findForcesBodies(i, m_root);
		}

		template<typename T>
		void ForceFinderGravityBH<T>::_deleteTree(Node* node)
		{
			if (node->children[0u] != nullptr)
				_deleteTree(node->children[0u]);

			if (node->children[1u] != nullptr)
				_deleteTree(node->children[1u]);

			if (node->children[2u] != nullptr)
				_deleteTree(node->children[2u]);

			if (node->children[3u] != nullptr)
				_deleteTree(node->children[3u]);

			if (node->children[4u] != nullptr)
				_deleteTree(node->children[4u]);

			if (node->children[5u] != nullptr)
				_deleteTree(node->children[5u]);

			if (node->children[6u] != nullptr)
				_deleteTree(node->children[6u]);

			if (node->children[7u] != nullptr)
				_deleteTree(node->children[7u]);

			delete node;
		}

		template<typename T>
		void ForceFinderGravityBH<T>::_findForceParallel(size_t start, size_t end)
		{
			for (size_t i = start; i < end; i++)
				if (_checkBodyInCube(i))
					_findForcesBodies(i, m_root);
		}

		template<typename T>
		void ForceFinderGravityBH<T>::_addBody(size_t index_new_body, Node* node)
		{
			if (node->body_index == NodeState::EMPTY)
			{
				node->body_index = index_new_body;
			}
			else if (node->body_index >= 0)
			{
				// move the body stored in current node to a new child node
				size_t octant_stored_body = _getOctant(
					this->m_gravity.position->operator[](0)[node->body_index],
					this->m_gravity.position->operator[](1)[node->body_index],
					this->m_gravity.position->operator[](2)[node->body_index],
					node->cube.x,
					node->cube.y,
					node->cube.z);

				node->children[octant_stored_body] = new Node(node->cube.x, node->cube.y, node->cube.z, node->cube.half_len, node->body_index, octant_stored_body);

				// store the new body in one of the children
				size_t octant_new_body = _getOctant(
					this->m_gravity.position->operator[](0)[index_new_body],
					this->m_gravity.position->operator[](1)[index_new_body],
					this->m_gravity.position->operator[](2)[index_new_body],
					node->cube.x,
					node->cube.y,
					node->cube.z);

				// we need to place the new body in the same child where we have just moved the stored body,
				// so we need to go deeper
				if (octant_new_body == octant_stored_body)
					_addBody(index_new_body, node->children[octant_stored_body]);
				else
					node->children[octant_new_body] = new Node(node->cube.x, node->cube.y, node->cube.z, node->cube.half_len, index_new_body, octant_new_body);

				node->body_index = NodeState::INTERNAL_NODE;

			}
			else if (node->body_index == NodeState::INTERNAL_NODE)
			{
				size_t octant_new_body = _getOctant(
					this->m_gravity.position->operator[](0)[index_new_body],
					this->m_gravity.position->operator[](1)[index_new_body],
					this->m_gravity.position->operator[](2)[index_new_body],
					node->cube.x,
					node->cube.y,
					node->cube.z);
				
				// if the new body needs to be put in a child that has not yet been created, we create the node
				// and add the body, else we go deeper
				if (node->children[octant_new_body] == nullptr)
					node->children[octant_new_body] = new Node(node->cube.x, node->cube.y, node->cube.z, node->cube.half_len, index_new_body, octant_new_body);
				else
					_addBody(index_new_body, node->children[octant_new_body]);
			}

		}

		template<typename T>
		void ForceFinderGravityBH<T>::_contributeCenterOfMass(size_t index, Node* node)
		{
			if (node->body_index == NodeState::INTERNAL_NODE)
			{
				node->center_mass.sum_x += this->m_gravity.mass->operator[](index) * this->m_gravity.position->operator[](0)[index];
				node->center_mass.sum_y += this->m_gravity.mass->operator[](index) * this->m_gravity.position->operator[](1)[index];
				node->center_mass.sum_z += this->m_gravity.mass->operator[](index) * this->m_gravity.position->operator[](2)[index];

				node->center_mass.mass += this->m_gravity.mass->operator[](index);

				size_t next_octant = _getOctant(
					this->m_gravity.position->operator[](0)[index],
					this->m_gravity.position->operator[](1)[index],
					this->m_gravity.position->operator[](2)[index],
					node->cube.x,
					node->cube.y,
					node->cube.z);

				_contributeCenterOfMass(index, node->children[next_octant]);
			}
			else if (node->body_index >= 0)
			{
				assert(node->body_index == index);
				
				node->center_mass.sum_x += this->m_gravity.mass->operator[](index) * this->m_gravity.position->operator[](0)[index];
				node->center_mass.sum_y += this->m_gravity.mass->operator[](index) * this->m_gravity.position->operator[](1)[index];
				node->center_mass.sum_z += this->m_gravity.mass->operator[](index) * this->m_gravity.position->operator[](2)[index];

				node->center_mass.mass += this->m_gravity.mass->operator[](index);

				return;
			}

		}

		template<typename T>
		void ForceFinderGravityBH<T>::_findForcesBodies(size_t index, const Node* node)
		{
			// length of the region
			const T s = node->cube.half_len * 2;

			// d = distance between body and center of mass
			const T com_x = node->center_mass.sum_x / node->center_mass.mass;
			const T com_y = node->center_mass.sum_y / node->center_mass.mass;
			const T com_z = node->center_mass.sum_z / node->center_mass.mass;
			
			const T diff_x = this->m_gravity.position->operator[](0)[index] - com_x;
			const T diff_y = this->m_gravity.position->operator[](1)[index] - com_y;
			const T diff_z = this->m_gravity.position->operator[](2)[index] - com_z;

			const T d_square = diff_x * diff_x + diff_y * diff_y + diff_z * diff_z;

			if((s * s) <= (m_theta * d_square) || node->body_index >= 0)
			{
				const T prod = -this->m_gravity.G * this->m_gravity.mass->operator[](index) * node->center_mass.mass;
				const T dist = sqrt(d_square);
				const T force_denominator = prod / (d_square * dist + this->m_gravity.soft);
				
				this->m_gravity.force->operator[](0)[index] += diff_x * force_denominator;
				this->m_gravity.force->operator[](1)[index] += diff_y * force_denominator;
				this->m_gravity.force->operator[](2)[index] += diff_z * force_denominator;
			}
			else
			{
				if (node->children[0u] != nullptr)
					_findForcesBodies(index, node->children[0u]);

				if (node->children[1u] != nullptr)
					_findForcesBodies(index, node->children[1u]);

				if (node->children[2u] != nullptr)
					_findForcesBodies(index, node->children[2u]);

				if (node->children[3u] != nullptr)
					_findForcesBodies(index, node->children[3u]);

				if (node->children[4u] != nullptr)
					_findForcesBodies(index, node->children[4u]);

				if (node->children[5u] != nullptr)
					_findForcesBodies(index, node->children[5u]);

				if (node->children[6u] != nullptr)
					_findForcesBodies(index, node->children[6u]);

				if (node->children[7u] != nullptr)
					_findForcesBodies(index, node->children[7u]);
			}

		}

		template<typename T>
		size_t ForceFinderGravityBH<T>::_getOctant(T body_x, T body_y, T body_z, T node_x, T node_y, T node_z)
		{
			if (body_x > node_x)
			{
				if (body_y > node_y)
				{
					if (body_z > node_z)
						return 0u;
					else
						return 4u;
				}
				else
				{
					if (body_z > node_z)
						return 2u;
					else
						return 6u;
				}
			}
			else
			{
				if (body_y > node_y)
				{
					if (body_z > node_z)
						return 1u;
					else
						return 5u;
				}
				else
				{
					if (body_z > node_z)
						return 3u;
					else
						return 7u;
				}
			}
			
			std::cout << "\nbody_x = " << body_x << " body_y = " << body_y << " body_z = " << body_z 
				<< "\nnode_x = " << node_x << " node_y = " << node_y << " node_z = " << node_z << std::endl;

			assert(false);
		}

		template<typename T>
		bool ForceFinderGravityBH<T>::_checkBodyInCube(size_t index)
		{
			if (std::abs(this->m_gravity.position->operator[](0)[index]) > m_largest_coord || 
				std::abs(this->m_gravity.position->operator[](1)[index]) > m_largest_coord || 
				std::abs(this->m_gravity.position->operator[](0)[index]) > m_largest_coord)
				return false;
			else
				return true;
		}

		template class ForceFinderGravityBH<float>;
		template class ForceFinderGravityBH<double>;
	}
}