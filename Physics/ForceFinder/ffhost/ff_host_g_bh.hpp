#pragma once

#include "ff_host_g_abstract.hpp"

#include <array>

#include <cassert>
#include <iostream>

namespace forcefinder
{
	namespace ffh
	{
		template<typename T>
		struct BarnesHutGravSpec
		{
			GravitySpec<T> gravity;
			T theta = 0;
		};

		template<typename T>
		class ForceFinderGravityBH : public AForceFinderGravity<T>
		{
		public:
			ForceFinderGravityBH(BarnesHutGravSpec<T>& init_struct);

			virtual void findForce() override;
			virtual double getDuration() override { return this->m_duration; }

		protected:

			double m_duration = 0;
			T m_theta = 0;

		private:

			T m_largest_coord = 0;

			enum NodeState
			{
				EMPTY = -1, ROOT = -2, INTERNAL_NODE = -3
			};

			struct CenterMass
			{
				T sum_x = 0;
				T sum_y = 0;
				T sum_z = 0;
				T mass = 0;
			};

			struct Cube
			{
				T x = 0;
				T y = 0;
				T z = 0;
				T half_len = 0;
			};

			struct Node
			{

				Node() {}
				Node(T px, T py, T pz, T phalf_len, size_t body_index, size_t octant)
				{
					this->body_index = body_index;
					cube.half_len = phalf_len / 2;

					assert(cube.half_len);

					switch (octant)
					{
					case 0u:
						cube.x = px + cube.half_len;
						cube.y = py + cube.half_len;
						cube.z = pz + cube.half_len;
						break;

					case 1u:
						cube.x = px - cube.half_len;
						cube.y = py + cube.half_len;
						cube.z = pz + cube.half_len;
						break;

					case 2u:
						cube.x = px + cube.half_len;
						cube.y = py - cube.half_len;
						cube.z = pz + cube.half_len;
						break;

					case 3u:
						cube.x = px - cube.half_len;
						cube.y = py - cube.half_len;
						cube.z = pz + cube.half_len;
						break;

					case 4u:
						cube.x = px + cube.half_len;
						cube.y = py + cube.half_len;
						cube.z = pz - cube.half_len;
						break;

					case 5u:
						cube.x = px - cube.half_len;
						cube.y = py + cube.half_len;
						cube.z = pz - cube.half_len;
						break;

					case 6u:
						cube.x = px + cube.half_len;
						cube.y = py - cube.half_len;
						cube.z = pz - cube.half_len;
						break;

					case 7u:
						cube.x = px - cube.half_len;
						cube.y = py - cube.half_len;
						cube.z = pz - cube.half_len;
						break;
					}
				}

				int64_t body_index = EMPTY;
				Cube cube;
				CenterMass center_mass;
				std::array<Node*, 8u> children{ nullptr };
			};

			Node* m_root = nullptr;

		private:

			std::vector<T>::iterator _maxElemVec(std::vector<T>& vec);
			void _findMostDistantCoord();

			void _createRoot();

			void _buildTree();
			void _findCenterOfMass();
			void _findForce();
			void _deleteTree(Node* node);

			void _findForceParallel(size_t start, size_t end);

			void _addBody(size_t index, Node* node);
			void _contributeCenterOfMass(size_t index, Node* node);
			void _findForcesBodies(size_t index, const Node* node);

			size_t _getOctant(T body_x, T body_y, T body_z, T node_x, T node_y, T node_z);
			bool _checkBodyInCube(size_t index);


		};


	}

}
