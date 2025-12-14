#pragma once

#include <array>
#include <cassert>

#include "Bodies.hpp"
#include "BodiesEnums.hpp"
#include "InitialConditions.hpp"

namespace body
{
	namespace glbody
	{
		using std::array;

		template<typename T>
		class Bodies : public IBodies<T>
		{
		public:
			Bodies(init::GenInitialConditions<T> initial_condition);
			Bodies(init::GenInitialConditions<T>& initial_condition);
			virtual ~Bodies() {}

			virtual void resize(init::GenInitialConditions<T>& initial_condition) override;
			virtual void reset(init::GenInitialConditions<T>& initial_condition) override;
			virtual CompressedGet get_properties() override;
			virtual void reset_force() override;

			PropertiesGL& get_prop() { return m_properties; }

		protected:

			PropertiesGL m_properties;

			unsigned int m_count = 0;

			void _init(init::GenInitialConditions<T>& initial_condition);
			void _set_buff_to_zero(core::VertexBuffer& vb);
		};

		
	}
}



