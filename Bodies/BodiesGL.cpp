#include "BodiesGL.hpp"

namespace body
{
	namespace glbody
	{
		template<typename T>
		Bodies<T>::Bodies(init::GenInitialConditions<T> initial_condition)
		{
			_init(initial_condition);
		}
		
		template<typename T>
		Bodies<T>::Bodies(init::GenInitialConditions<T>& initial_condition)
		{
			_init(initial_condition);
		}

		template<typename T>
		void Bodies<T>::resize(init::GenInitialConditions<T>& initial_condition)
		{
			size_t number_bodies = initial_condition.get_count();

			m_properties.position[0].reallocVB(number_bodies * sizeof(T));
			m_properties.position[1].reallocVB(number_bodies * sizeof(T));
			m_properties.position[2].reallocVB(number_bodies * sizeof(T));

			m_properties.velocity[0].reallocVB(number_bodies * sizeof(T));
			m_properties.velocity[1].reallocVB(number_bodies * sizeof(T));
			m_properties.velocity[2].reallocVB(number_bodies * sizeof(T));

			m_properties.mass[0].reallocVB(number_bodies * sizeof(T));

			m_properties.acceleration[0].reallocVB(number_bodies * sizeof(T));
			m_properties.acceleration[1].reallocVB(number_bodies * sizeof(T));
			m_properties.acceleration[2].reallocVB(number_bodies * sizeof(T));

			m_properties.force[0].reallocVB(number_bodies * sizeof(T));
			m_properties.force[1].reallocVB(number_bodies * sizeof(T));
			m_properties.force[2].reallocVB(number_bodies * sizeof(T));

			if (m_count < number_bodies)
			{
				size_t diff_count = number_bodies - static_cast<size_t>(m_count);

				init::InitResult<T> init_result = initial_condition.get();

				m_properties.position[0].updateVB(sizeof(T) * m_count, sizeof(T) * diff_count, init_result.position[0].data() + m_count);
				m_properties.position[1].updateVB(sizeof(T) * m_count, sizeof(T) * diff_count, init_result.position[1].data() + m_count);
				m_properties.position[2].updateVB(sizeof(T) * m_count, sizeof(T) * diff_count, init_result.position[2].data() + m_count);

				m_properties.velocity[0].updateVB(sizeof(T) * m_count, sizeof(T) * diff_count, init_result.velocity[0].data() + m_count);
				m_properties.velocity[1].updateVB(sizeof(T) * m_count, sizeof(T) * diff_count, init_result.velocity[1].data() + m_count);
				m_properties.velocity[2].updateVB(sizeof(T) * m_count, sizeof(T) * diff_count, init_result.velocity[2].data() + m_count);

				m_properties.mass[0].updateVB(sizeof(T) * m_count, sizeof(T) * diff_count, init_result.mass[0].data() + m_count);
			}

			m_count = number_bodies;
		}

		template<typename T>
		void Bodies<T>::reset(init::GenInitialConditions<T>& initial_condition)
		{
			init::InitResult<T> init_result = initial_condition.get();

			m_properties.position[0].updateVB(sizeof(T) * m_count, init_result.position[0].data());
			m_properties.position[1].updateVB(sizeof(T) * m_count, init_result.position[1].data());
			m_properties.position[2].updateVB(sizeof(T) * m_count, init_result.position[2].data());

			m_properties.velocity[0].updateVB(sizeof(T) * m_count, init_result.velocity[0].data());
			m_properties.velocity[1].updateVB(sizeof(T) * m_count, init_result.velocity[1].data());
			m_properties.velocity[2].updateVB(sizeof(T) * m_count, init_result.velocity[2].data());

			m_properties.mass[0].updateVB(sizeof(T) * m_count, init_result.mass[0].data());

			m_properties.acceleration[0].updateVB(sizeof(T) * m_count, init_result.acceleration[0].data());
			m_properties.acceleration[1].updateVB(sizeof(T) * m_count, init_result.acceleration[1].data());
			m_properties.acceleration[2].updateVB(sizeof(T) * m_count, init_result.acceleration[2].data());

			m_properties.force[0].updateVB(sizeof(T) * m_count, init_result.force[0].data());
			m_properties.force[1].updateVB(sizeof(T) * m_count, init_result.force[1].data());
			m_properties.force[2].updateVB(sizeof(T) * m_count, init_result.force[2].data());
		}

		template<typename T>
		CompressedGet Bodies<T>::get_properties()
		{
			CompressedGet prop_get(m_properties);

			return prop_get;
		}

		template<typename T>
		void Bodies<T>::reset_force()
		{
			_set_buff_to_zero(m_properties.force[0]);
			_set_buff_to_zero(m_properties.force[1]);
			_set_buff_to_zero(m_properties.force[2]);
		}

		template<typename T>
		void Bodies<T>::_init(init::GenInitialConditions<T>& initial_condition)
		{
			init::InitResult<T> init_result = initial_condition.get();
			m_count = init_result.count;

			m_properties.position[0] = core::VertexBuffer(sizeof(T) * m_count, init_result.position[0].data());
			m_properties.position[1] = core::VertexBuffer(sizeof(T) * m_count, init_result.position[1].data());
			m_properties.position[2] = core::VertexBuffer(sizeof(T) * m_count, init_result.position[2].data());

			m_properties.velocity[0] = core::VertexBuffer(sizeof(T) * m_count, init_result.velocity[0].data());
			m_properties.velocity[1] = core::VertexBuffer(sizeof(T) * m_count, init_result.velocity[1].data());
			m_properties.velocity[2] = core::VertexBuffer(sizeof(T) * m_count, init_result.velocity[2].data());

			m_properties.mass[0] = core::VertexBuffer(sizeof(T) * m_count, init_result.mass[0].data());

			m_properties.acceleration[0] = core::VertexBuffer(sizeof(T) * m_count, init_result.acceleration[0].data());
			m_properties.acceleration[1] = core::VertexBuffer(sizeof(T) * m_count, init_result.acceleration[1].data());
			m_properties.acceleration[2] = core::VertexBuffer(sizeof(T) * m_count, init_result.acceleration[2].data());

			m_properties.force[0] = core::VertexBuffer(sizeof(T) * m_count, init_result.force[0].data());
			m_properties.force[1] = core::VertexBuffer(sizeof(T) * m_count, init_result.force[1].data());
			m_properties.force[2] = core::VertexBuffer(sizeof(T) * m_count, init_result.force[2].data());

		}

		template<typename T>
		void Bodies<T>::_set_buff_to_zero(core::VertexBuffer& vb)
		{
			T* ptr = static_cast<T*>(vb.mapBuffer(GL_WRITE_ONLY));

			assert(ptr);

			for (size_t i = 0; i < m_count; i++)
				ptr[i] = static_cast<T>(0);

			GLboolean result_state = vb.unmapBuffer();
			assert(result_state);	 
		}

		template class Bodies<GLfloat>;
		template class Bodies<GLdouble>;
	}

}
