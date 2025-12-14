#pragma once

#include <vector>
#include <array>
#include <variant>

#include "InitialConditions.hpp"

#include "VertexBuffer.hpp"

namespace body
{
	template<typename T>
	struct PropertiesHost
	{
		std::vector<std::vector<T>> position;
		std::vector<std::vector<T>> velocity;
		std::vector<std::vector<T>> acceleration;
		std::vector<std::vector<T>> force;
		std::vector<std::vector<T>> mass;
	};

	struct PropertiesGL
	{
		std::array<core::VertexBuffer, 3u> position;
		std::array<core::VertexBuffer, 3u> velocity;
		std::array<core::VertexBuffer, 3u> acceleration;
		std::array<core::VertexBuffer, 3u> force;
		std::array<core::VertexBuffer, 1u> mass;
	};


	class CompressedGet
	{
	public:
		template<typename T>
		CompressedGet(PropertiesHost<T>& prop_h);
		CompressedGet(PropertiesGL& prop_gl);

		enum class ActiveGet
		{
			PROP_H_32,
			PROP_H_64,
			PROP_GL
		};

		ActiveGet getActiveType();

		template<typename T>
		PropertiesHost<T>* getPropHost();
		PropertiesGL* getCompGl();

	private:

		std::variant<PropertiesHost<float>*, PropertiesHost<double>*, PropertiesGL*> m_internal_struct;

		ActiveGet m_active_type;

		template<typename T>
		ActiveGet _getPropTypeHost();
	};
	
	
	template<typename T>
	class IBodies
	{
	public:

		virtual ~IBodies() {}

		virtual void resize(init::GenInitialConditions<T>& initial_condition) = 0;
		virtual void reset(init::GenInitialConditions<T>& initial_condition) = 0;
		virtual CompressedGet get_properties() = 0;
		virtual void reset_force() = 0;
	};
}
