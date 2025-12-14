#include "ff_gl_g_direct.hpp"

#include "ResourceManager.hpp"
#include "ResourceNames.hpp"
#include "Timer.hpp"

#include <iostream>

namespace forcefinder
{
	template<typename T>
	struct FFdata
	{
		T g;
		T soft;
		GLuint number_bodies;
	};
	
	namespace ffgl
	{
		template<typename T>
		ForceFinderGravityDirect<T>::ForceFinderGravityDirect(GravitySpec<T>& init_struct)
			: 
		AForceFinderGravity<T>(init_struct)
		{
			m_data = core::VertexBuffer(sizeof(GLuint) + 2u * sizeof(T), nullptr);

			m_data.updateVB(0,             sizeof(T),      &init_struct.G);
			m_data.updateVB(sizeof(T),     sizeof(T),      &init_struct.soft);
			m_data.updateVB(2 * sizeof(T), sizeof(GLuint), &init_struct.count);

			this->m_compute_shader = &getShaderResource();
		}

		template<typename T>
		void ForceFinderGravityDirect<T>::findForce()
		{
			this->m_data.bindBufferBase(0u);

			this->m_gravity.vbo_position->operator[](0).bindBufferBase(1u);
			this->m_gravity.vbo_position->operator[](1).bindBufferBase(2u);
			this->m_gravity.vbo_position->operator[](2).bindBufferBase(3u);

			this->m_gravity.vbo_force->operator[](0).bindBufferBase(4u);
			this->m_gravity.vbo_force->operator[](1).bindBufferBase(5u);
			this->m_gravity.vbo_force->operator[](2).bindBufferBase(6u);

			this->m_gravity.vbo_mass->bindBufferBase(7u);

			const GLuint size_dispatch = this->m_gravity.count / 64u;

			this->m_compute_shader->use();

			timer::Timer timer;
			glDispatchCompute(size_dispatch, 1u, 1u);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			glFinish();
			this->m_duration = timer.getDuration();

			this->m_compute_shader->unuse();
		}

		template<>
		core::Shader& ForceFinderGravityDirect<float>::getShaderResource()
		{
			return core::ResourceManager::getShader(nbody::csname::forcefinder_f32);
		}

		template<>
		core::Shader& ForceFinderGravityDirect<double>::getShaderResource()
		{
			return core::ResourceManager::getShader(nbody::csname::forcefinder_f64);
		}

		template class ForceFinderGravityDirect<float>;
		template class ForceFinderGravityDirect<double>;

	}
}

