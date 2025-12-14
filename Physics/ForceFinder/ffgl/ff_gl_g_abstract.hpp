#pragma once

#include "ff_gl.hpp"

namespace forcefinder
{
	namespace ffgl
	{
		template<typename T>
		struct GravitySpec
		{
			T G = 0;
			T soft = 0;
			GLuint count = 0;

			std::array<core::VertexBuffer, 3>* vbo_position = nullptr;
			std::array<core::VertexBuffer, 3>* vbo_force = nullptr;
			core::VertexBuffer* vbo_mass = nullptr;
		};
		
		template<typename T>
		class AForceFinderGravity : public IForceFinder
		{
		public:
			AForceFinderGravity(GravitySpec<T>& init_struct);
			virtual ~AForceFinderGravity() {}

			virtual void setBodiesProperties(ForceFinderData& gl_properties) override;
			virtual double getDuration() override;

		protected:

			GravitySpec<T> m_gravity;
			double m_duration;
		};

	}
}