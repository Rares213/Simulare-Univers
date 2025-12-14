#pragma once

#include "ff_gl_g_abstract.hpp"
#include "VertexBuffer.hpp"

#include "Shader.hpp"

namespace forcefinder
{
	namespace ffgl
	{
		template<typename T>
		class ForceFinderGravityDirect : public AForceFinderGravity<T>
		{
		public:
			ForceFinderGravityDirect(GravitySpec<T>& init_struct);
			virtual ~ForceFinderGravityDirect() {}

			virtual void findForce() override;

		protected:

			core::VertexBuffer m_data;
			core::Shader* m_compute_shader;

		private:

			core::Shader& getShaderResource();
		};
	}
}