#pragma once

#include "BodiesEnums.hpp"

#include <vector>

namespace body
{
	namespace init
	{
		template<typename T>
		struct InitResult
		{
			InitResult(InitialConditions init_type, size_t count);
			
			std::vector<std::vector<T>> position;
			std::vector<std::vector<T>> velocity;
			std::vector<std::vector<T>> acceleration;
			std::vector<std::vector<T>> force;
			std::vector<std::vector<T>> mass;

			long long int count;
			InitialConditions init_type;
		};
		
		template<typename T>
		class GenInitialConditions
		{
		public:
			GenInitialConditions(InitialConditions init_type, size_t count);
			GenInitialConditions(InitResult<T> custom_init);

			InitResult<T> get() { return m_initial_cond; }
			long long int get_count() { return m_initial_cond.count; }

		private:

			InitResult<T> m_initial_cond;

			void _init();

			void _static();
			void _spiral();
			void _test_bh();
			
			void _static_explicit(size_t number_dimensions);
			void _velocity_spiral();
			void _init_mass();
		};



	}
}