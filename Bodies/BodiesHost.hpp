#pragma once


#include "Bodies.hpp"

namespace body
{
	namespace hbody
	{
		template <typename T> 
		class Bodies : public IBodies<T>
		{
		public:
			Bodies(init::GenInitialConditions<T>& init_cond);
			Bodies(init::GenInitialConditions<T>&& init_cond);
			virtual ~Bodies() {}

			virtual void resize(init::GenInitialConditions<T>& initial_condition) override;
			virtual void reset(init::GenInitialConditions<T>& initial_condition) override;
			virtual CompressedGet get_properties() override;
			virtual void reset_force() override;

			PropertiesHost<T>& get_prop();

			void printDebug();

		private:
			PropertiesHost<T> m_properties;

			size_t m_count = 0;

			void _init(init::GenInitialConditions<T>& init_cond);
		};

	}
}
