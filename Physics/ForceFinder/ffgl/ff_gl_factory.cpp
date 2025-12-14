#include "ff_gl_factory.hpp"

namespace forcefinder
{
	namespace ffgl
	{
		template<typename T>
		std::unique_ptr<IForceFinder> FactoryForceFinderGravityDirect<T>::createForceFinder()
		{
			GravitySpec<T> data
			{
				m_data.G,
				m_data.soft,
				m_data.count,
				&m_data.vbo_position,
				&m_data.vbo_force,
				&m_data.vbo_mass
			};

			return std::make_unique<ForceFinderGravityDirect<T>>(data);
		}

		template class FactoryForceFinderGravityDirect<float>;
		template class FactoryForceFinderGravityDirect<double>;
	}
}