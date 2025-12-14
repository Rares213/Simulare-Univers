#pragma once

#include "glad/glad.h"
#include "VertexBuffer.hpp"

#include <array>

namespace forcefinder
{
	namespace ffgl
	{
		struct ForceFinderData
		{
			std::array<core::VertexBuffer, 3>* vbo_position;
			std::array<core::VertexBuffer, 3>* vbo_velocity;
			std::array<core::VertexBuffer, 3>* vbo_acceleration;
			std::array<core::VertexBuffer, 3>* vbo_force;
			core::VertexBuffer* vbo_mass;
		};
		
		
		class IForceFinder
		{
		public:
			virtual ~IForceFinder() {}

			virtual void findForce() = 0;
			virtual void setBodiesProperties(ForceFinderData& gl_properties) = 0;
			virtual double getDuration() = 0;
		};


		

	}

}