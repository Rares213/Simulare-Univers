#pragma once

#include "../../ForceFinder/ffgl/ff_gl.hpp"

#include <array>

#include "glad/glad.h"
#include "VertexBuffer.hpp"
#include "Shader.hpp"


namespace integrator
{
	namespace igl
	{
		template<typename T> 
		struct IntegratorData
		{
			GLuint count = 0;
			T stepsize = 0;

			std::array<core::VertexBuffer, 3u>& vbo_position;
			std::array<core::VertexBuffer, 3u>& vbo_velocity;
			std::array<core::VertexBuffer, 3u>& vbo_acceleration;
			std::array<core::VertexBuffer, 3u>& vbo_force;
			core::VertexBuffer& vbo_mass;

		};
		
		class IIntegrator
		{
		public:

			virtual ~IIntegrator() {}

			virtual void updatePosition(forcefinder::ffgl::IForceFinder& force_finder) = 0;
			virtual double getDuration() = 0;
		};

		template<typename T>
		class EulerImplicit : public IIntegrator
		{
		public:
			EulerImplicit(const IntegratorData<T>& init_struct);
			virtual ~EulerImplicit() {}

			virtual void updatePosition(forcefinder::ffgl::IForceFinder& force_finder) override;
			virtual double getDuration() override { return m_duration; }

		private:

			double m_duration = 0.0;

			IntegratorData<T> m_data;
			core::VertexBuffer m_cs_data;
			core::Shader* m_cs;

			core::Shader& getComputeShader();

			void _dfillVectorWithVbo(std::vector<T>& vec, core::VertexBuffer& vb);
		};

		template<typename T>
		class LeapFrog : public IIntegrator
		{
		public:
			LeapFrog(const IntegratorData<T>& data);
			virtual ~LeapFrog() {}

			virtual void updatePosition(forcefinder::ffgl::IForceFinder& force_finder) override;
			virtual double getDuration() override { return m_duration; }

		private:

			double m_duration = 0.0;

			GLuint size_dispatch;

			IntegratorData<T> m_data;
			core::VertexBuffer m_cs_data;
			std::array<core::VertexBuffer, 3u> m_prev_acc;

			core::Shader* m_acc_cs;

			core::Shader* m_leapfrog_first_stage_cs;
			core::Shader* m_leapfrog_second_stage_cs;

			void _loadComputeShaders();

			void _firstStage();
			void _intermediaryStage(forcefinder::ffgl::IForceFinder& force_finder);
			void _secondStage();
		};
	}


}
