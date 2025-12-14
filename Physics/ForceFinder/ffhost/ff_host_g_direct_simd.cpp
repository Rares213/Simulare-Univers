#include "ff_host_g_direct.hpp"

#include <array>
#include "immintrin.h"
#include "Timer.hpp"

namespace forcefinder
{
	namespace ffh
	{
		template<typename T>
		ForceFinderGravityDirectSIMD<T>::ForceFinderGravityDirectSIMD(GravitySpec<T>& init_stuct)
			:
		ForceFinderGravityDirect<T>(init_stuct)
		{
			m_vectorizable_samples = getVectorizableSize();

			this->m_task_manager.clear();

			std::vector<WorkerData> workers_data = commfn::getWorkersData(init_stuct.mass->size(), init_stuct.workers);

			for (size_t i = 0; i < workers_data.size(); i++)
			{
				std::function<void(void)> worker_fun = std::bind(&ForceFinderGravityDirectSIMD::_gravitySimd, this, workers_data[i].start, workers_data[i].end);
				this->m_task_manager.add_task(i, std::move(worker_fun), true);
			}
		}

		template<typename T>
		void forcefinder::ffh::ForceFinderGravityDirectSIMD<T>::findForce()
		{
			timer::Timer timer;

			if (this->m_workers_count == 0)
				_gravitySimd(0, this->m_count);
			else
			{
				for (size_t i = 0; i < this->m_workers_count; i++)
					this->m_task_manager.launch_task(i);

				this->m_task_manager.wait_all_finished();
			}

			this->m_duration = timer.getDuration();
		}

		template<>
		void ForceFinderGravityDirectSIMD<float>::_gravitySimd(size_t start, size_t end)
		{
			using std::vector;

			const size_t size = this->m_count;
			const size_t unvectorizable_elements = size % m_vectorizable_samples;
			const size_t vectorizable_size = size - unvectorizable_elements;

			const vector<float>& x = this->m_gravity.position->operator[](0);
			const vector<float>& y = this->m_gravity.position->operator[](1);
			const vector<float>& z = this->m_gravity.position->operator[](2);

			const vector<float>& mass = *this->m_gravity.mass;

			vector<float>& fx = this->m_gravity.force->operator[](0);
			vector<float>& fy = this->m_gravity.force->operator[](1);
			vector<float>& fz = this->m_gravity.force->operator[](2);

			const float G    = this->m_gravity.G;
			const float soft = this->m_gravity.soft;

			float accum_fx, accum_fy, accum_fz;

			std::array<float, 8> forcesXYZ{ 0.0f };
			std::array<__m256, 3> distCurrentBody;
			std::array<__m256, 3> distDifferenceBodies;
			std::array<__m256, 3> distOtherBodies;
			std::array<__m256, 3> distCompSquared;
			std::array<__m256i, 2> permuteIndices;

			__m256 massCurrentBody, massBodies, distance, GMassProd, FX, FY, FZ, sG, GMassDivDist, sumDistComp;
			__m256 forcesSum, sSoft;

			sG = _mm256_set1_ps(-G);
			permuteIndices[0] = _mm256_set_epi32(7, 6, 5, 1, 3, 2, 4, 0);
			permuteIndices[1] = _mm256_set_epi32(7, 6, 3, 4, 5, 1, 2, 0);
			sSoft = _mm256_set1_ps(soft);

			for (size_t i = start; i < end; i++)
			{
				accum_fx = accum_fy = accum_fz = 0.0f;

				distCurrentBody[0] = _mm256_set1_ps(x[i]);
				distCurrentBody[1] = _mm256_set1_ps(y[i]);
				distCurrentBody[2] = _mm256_set1_ps(z[i]);
				massCurrentBody = _mm256_set1_ps(mass[i]);

				for (size_t j = 0; j < vectorizable_size; j += m_vectorizable_samples)
				{
					distOtherBodies[0] = _mm256_loadu_ps(x.data() + j);
					distOtherBodies[1] = _mm256_loadu_ps(y.data() + j);
					distOtherBodies[2] = _mm256_loadu_ps(z.data() + j);
					massBodies = _mm256_loadu_ps(mass.data() + j);

					distDifferenceBodies[0] = _mm256_sub_ps(distCurrentBody[0], distOtherBodies[0]);
					distDifferenceBodies[1] = _mm256_sub_ps(distCurrentBody[1], distOtherBodies[1]);
					distDifferenceBodies[2] = _mm256_sub_ps(distCurrentBody[2], distOtherBodies[2]);

					distCompSquared[0] = _mm256_mul_ps(distDifferenceBodies[0], distDifferenceBodies[0]);
					distCompSquared[1] = _mm256_mul_ps(distDifferenceBodies[1], distDifferenceBodies[1]);
					distCompSquared[2] = _mm256_mul_ps(distDifferenceBodies[2], distDifferenceBodies[2]);

					sumDistComp = _mm256_add_ps(_mm256_add_ps(distCompSquared[0], distCompSquared[1]), distCompSquared[2]);
					sumDistComp = _mm256_add_ps(sumDistComp, sSoft);
					distance = _mm256_mul_ps(sumDistComp, _mm256_sqrt_ps(sumDistComp));

					GMassProd = _mm256_mul_ps(sG, _mm256_mul_ps(massCurrentBody, massBodies));
					GMassDivDist = _mm256_div_ps(GMassProd, distance);

					FX = _mm256_mul_ps(GMassDivDist, distDifferenceBodies[0]);
					FY = _mm256_mul_ps(GMassDivDist, distDifferenceBodies[1]);
					FZ = _mm256_mul_ps(GMassDivDist, distDifferenceBodies[2]);

					forcesSum = _mm256_hadd_ps(FX, FY);
					forcesSum = _mm256_hadd_ps(forcesSum, FZ);
					forcesSum = _mm256_permutevar8x32_ps(forcesSum, permuteIndices[0]);
					forcesSum = _mm256_hadd_ps(forcesSum, _mm256_set1_ps(0.0f));
					forcesSum = _mm256_permutevar8x32_ps(forcesSum, permuteIndices[1]);
					forcesSum = _mm256_hadd_ps(forcesSum, _mm256_set1_ps(0.0f));

					_mm256_storeu_ps(forcesXYZ.data(), forcesSum);

					accum_fx += forcesXYZ[0];
					accum_fy += forcesXYZ[4];
					accum_fz += forcesXYZ[1];
				}

				fx[i] += accum_fx;
				fy[i] += accum_fy;
				fz[i] += accum_fz;
			}

			if (vectorizable_size != size)
				ForceFinderGravityDirect<float>::_gravityExplicit(start, end, this->m_count - unvectorizable_elements, this->m_count);
		}

		template<>
		void ForceFinderGravityDirectSIMD<double>::_gravitySimd(size_t start, size_t end)
		{

		}

		template<>
		size_t ForceFinderGravityDirectSIMD<float>::getVectorizableSize()
		{
			return 8u;
		}

		template<>
		size_t ForceFinderGravityDirectSIMD<double>::getVectorizableSize()
		{
			return 4u;
		}


	}
}

template class forcefinder::ffh::ForceFinderGravityDirectSIMD<float>;
template class forcefinder::ffh::ForceFinderGravityDirectSIMD<double>; 