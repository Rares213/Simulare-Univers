#pragma once

#include "ff_host_g_abstract.hpp"


namespace forcefinder
{
	namespace ffh
	{
		//direct algorithm for gravity
		template<typename T>
		class ForceFinderGravityDirect : public AForceFinderGravity<T>
		{
		public:
			ForceFinderGravityDirect(GravitySpec<T>& init_stuct);
			virtual ~ForceFinderGravityDirect() {}

			virtual void findForce() override;
			virtual double getDuration() override { return this->m_duration; }

		protected:

			void _gravity(size_t start, size_t end);
			void _gravityExplicit(size_t start, size_t end, size_t interacting_start, size_t interacting_end);
		};


		//direct algorithm for gravity that uses SIMD

		template<typename T>
		class ForceFinderGravityDirectSIMD : public ForceFinderGravityDirect<T>
		{
		public:
			ForceFinderGravityDirectSIMD(GravitySpec<T>& init_stuct);

			virtual ~ForceFinderGravityDirectSIMD() {}

			virtual void findForce() override;

		protected:

			void _gravitySimd(size_t start, size_t end);

		private:

			size_t m_vectorizable_samples = 0;

			size_t getVectorizableSize();

		};

		template<> void ForceFinderGravityDirectSIMD<float>::_gravitySimd(size_t start, size_t end);
		template<> void ForceFinderGravityDirectSIMD<double>::_gravitySimd(size_t start, size_t end);

		template<> size_t ForceFinderGravityDirectSIMD<float>::getVectorizableSize();
		template<> size_t ForceFinderGravityDirectSIMD<double>::getVectorizableSize();
	}
}