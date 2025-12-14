#include "integrator_gl.hpp"

#include "ResourceManager.hpp"
#include "ResourceNames.hpp"
#include "Timer.hpp"

namespace integrator
{
	namespace igl
	{
		template<typename T>
		LeapFrog<T>::LeapFrog(const IntegratorData<T>& data)
			: m_data(data)
		{
			size_dispatch = m_data.count / 64u;
			
			m_cs_data = core::VertexBuffer(sizeof(T), &m_data.stepsize);
			m_prev_acc[0] = core::VertexBuffer(sizeof(T) * m_data.count, nullptr);
			m_prev_acc[1] = core::VertexBuffer(sizeof(T) * m_data.count, nullptr);
			m_prev_acc[2] = core::VertexBuffer(sizeof(T) * m_data.count, nullptr);

			_loadComputeShaders();
		}

		template<typename T>
		void LeapFrog <T>::updatePosition(forcefinder::ffgl::IForceFinder& force_finder)
		{
			_firstStage();
			_intermediaryStage(force_finder);
			_secondStage();
		}

		template<>
		void LeapFrog<float>::_loadComputeShaders()
		{
			m_acc_cs = &core::ResourceManager::getShader(nbody::csname::acc_f32);
			m_leapfrog_first_stage_cs = &core::ResourceManager::getShader(nbody::csname::leapfrog_first_stage_f32);
			m_leapfrog_second_stage_cs = &core::ResourceManager::getShader(nbody::csname::leapfrog_second_stage_f32);
		}

		template<>
		void LeapFrog<double>::_loadComputeShaders()
		{
			m_acc_cs = &core::ResourceManager::getShader(nbody::csname::acc_f64);
			m_leapfrog_first_stage_cs = &core::ResourceManager::getShader(nbody::csname::leapfrog_first_stage_f64);
			m_leapfrog_second_stage_cs = &core::ResourceManager::getShader(nbody::csname::leapfrog_second_stage_f64);
		}

		template<typename T>
		void LeapFrog<T>::_firstStage()
		{
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

			m_leapfrog_first_stage_cs->use();

			glDispatchCompute(size_dispatch, 1u, 1u);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			glFinish();

			m_duration = timer.getDuration();
		}

		template<typename T>
		void LeapFrog<T>::_intermediaryStage(forcefinder::ffgl::IForceFinder& force_finder)
		{
			force_finder.findForce();

			timer::Timer timer;
			// save acc for second stage
			m_prev_acc[0].copyBuffer(m_data.vbo_acceleration[0]);
			m_prev_acc[1].copyBuffer(m_data.vbo_acceleration[1]);
			m_prev_acc[2].copyBuffer(m_data.vbo_acceleration[2]);

			// find new acc
			m_data.vbo_acceleration[0].bindBufferBase(0u);
			m_data.vbo_acceleration[1].bindBufferBase(1u);
			m_data.vbo_acceleration[2].bindBufferBase(2u);

			m_data.vbo_force[0].bindBufferBase(3u);
			m_data.vbo_force[1].bindBufferBase(4u);
			m_data.vbo_force[2].bindBufferBase(5u);

			m_data.vbo_mass.bindBufferBase(6u);

			m_acc_cs->use();

			glDispatchCompute(size_dispatch, 1u, 1u);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			glFinish();

			m_duration += timer.getDuration();
		}

		template<typename T>
		void LeapFrog<T>::_secondStage()
		{
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

			m_prev_acc[0].bindBufferBase(10u);
			m_prev_acc[1].bindBufferBase(11u);
			m_prev_acc[2].bindBufferBase(12u);

			m_leapfrog_second_stage_cs->use();

			glDispatchCompute(size_dispatch, 1u, 1u);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			glFinish();

			m_leapfrog_second_stage_cs->unuse();

			m_duration += timer.getDuration();
		}

		template class LeapFrog<float>;
		template class LeapFrog<double>;
	}
}