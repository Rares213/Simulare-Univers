#include "integrator_gl.hpp"

#include "ResourceManager.hpp"
#include "ResourceNames.hpp"
#include "Timer.hpp"


namespace integrator
{
	
	namespace igl
	{
		template<typename T>
		EulerImplicit<T>::EulerImplicit(const IntegratorData<T>& init_struct)
			: m_data(init_struct)
		{
			m_cs_data = core::VertexBuffer(sizeof(T), &init_struct.stepsize);

			m_cs = &getComputeShader();
		}

		template<typename T>
		void EulerImplicit<T>::updatePosition(forcefinder::ffgl::IForceFinder& force_finder)
		{
			force_finder.findForce();

			timer::Timer timer;
			m_cs_data.bindBufferBase(0u);

			m_data.vbo_position[0].bindBufferBase(1u);
			m_data.vbo_position[1].bindBufferBase(2u);
			m_data.vbo_position[2].bindBufferBase(3u);

			m_data.vbo_velocity[0].bindBufferBase(4u);
			m_data.vbo_velocity[1].bindBufferBase(5u);
			m_data.vbo_velocity[2].bindBufferBase(6u);

			m_data.vbo_acceleration[0].bindBufferBase(7u);
			m_data.vbo_acceleration[1].bindBufferBase(8u);
			m_data.vbo_acceleration[2].bindBufferBase(9u);

			m_data.vbo_force[0].bindBufferBase(10u);
			m_data.vbo_force[1].bindBufferBase(11u);
			m_data.vbo_force[2].bindBufferBase(12u);

			m_data.vbo_mass.bindBufferBase(13u);

			const GLuint size_dispatch = m_data.count / 64u;
			
			m_cs->use();

			glDispatchCompute(size_dispatch, 1u, 1u);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			glFinish();

			m_cs->unuse();

			m_duration = timer.getDuration();
		}

		template<>
		core::Shader& integrator::igl::EulerImplicit<float>::getComputeShader()
		{
			return core::ResourceManager::getShader(nbody::csname::euler_implicit_f32);
		}

		template<>
		core::Shader& integrator::igl::EulerImplicit<double>::getComputeShader()
		{
			return core::ResourceManager::getShader(nbody::csname::euler_implicit_f64);
		}

		template<typename T>
		void EulerImplicit<T>::_dfillVectorWithVbo(std::vector<T>& vec, core::VertexBuffer& vb)
		{
			T* data = (T*)vb.mapBuffer(GL_READ_ONLY);

			for (size_t i = 0; i < m_data.count; i++)
				vec[i] = data[i];

			vb.unmapBuffer();
		}

		template class EulerImplicit<float>;
		template class EulerImplicit<double>;
	}
}

