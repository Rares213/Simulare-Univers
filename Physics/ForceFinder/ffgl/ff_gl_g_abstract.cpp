#include "ff_gl_g_abstract.hpp"

namespace forcefinder
{
	namespace ffgl
	{
		template<typename T>
		AForceFinderGravity<T>::AForceFinderGravity(GravitySpec<T>& init_struct)
			: m_gravity(init_struct)
		{

		}

		template<typename T>
		void AForceFinderGravity<T>::setBodiesProperties(ForceFinderData& gl_properties)
		{
			m_gravity.vbo_position = gl_properties.vbo_position;
		}

		template<typename T>
		double AForceFinderGravity<T>::getDuration()
		{
			return m_duration;
		}

		template class AForceFinderGravity<float>;
		template class AForceFinderGravity<double>;
	}
}