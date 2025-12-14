#pragma once

#include "ff_gl.hpp"
#include "ff_gl_g_direct.hpp"

#include "VertexBuffer.hpp"

#include <memory>

namespace forcefinder
{
	namespace ffgl
	{
		template<typename T>
		struct FactoryGravityData
		{
			float& G;
			float& soft;
			size_t& count;

			std::array<core::VertexBuffer, 3u>& vbo_position;
			std::array<core::VertexBuffer, 3u>& vbo_force;
			core::VertexBuffer& vbo_mass;
		};
		
		template <typename T>
		class IFactoryForceFinder
		{
		public:
			virtual ~IFactoryForceFinder() {}

			virtual std::unique_ptr<IForceFinder> createForceFinder() = 0;
		};

		template<typename T>
		class FactoryForceFinderGravityDirect : public IFactoryForceFinder<T>
		{
		public:

			FactoryForceFinderGravityDirect(FactoryGravityData<T>& data) : m_data(data) {}
			virtual ~FactoryForceFinderGravityDirect() {}

			virtual std::unique_ptr<IForceFinder> createForceFinder() override;

		private:

			FactoryGravityData<T> m_data;
		};


	}
}
