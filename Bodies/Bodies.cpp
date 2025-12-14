#include "bodies.hpp"

namespace body
{
	template<typename T>
	body::CompressedGet::CompressedGet(PropertiesHost<T>& prop_h)
	{
		m_active_type = _getPropTypeHost<T>();
	}

	CompressedGet::CompressedGet(PropertiesGL& prop_gl)
	{
		m_active_type = ActiveGet::PROP_GL;

		m_internal_struct = &prop_gl;
	}

	CompressedGet::ActiveGet CompressedGet::getActiveType()
	{
		return m_active_type;
	}

	template<>
	PropertiesHost<float>* CompressedGet::getPropHost<float>()
	{
		return std::get<PropertiesHost<float>*>(m_internal_struct);
	}

	template<>
	PropertiesHost<double>* CompressedGet::getPropHost<double>()
	{
		return std::get<PropertiesHost<double>*>(m_internal_struct);
	}

	PropertiesGL* CompressedGet::getCompGl()
	{
		return std::get<PropertiesGL*>(m_internal_struct);
	}

	template<>
	CompressedGet::ActiveGet CompressedGet::_getPropTypeHost<float>()
	{
		return ActiveGet::PROP_H_32;
	}

	template<>
	CompressedGet::ActiveGet CompressedGet::_getPropTypeHost<double>()
	{
		return ActiveGet::PROP_H_64;
	}

	template body::CompressedGet::CompressedGet(PropertiesHost<float>& prop_h);
	template body::CompressedGet::CompressedGet(PropertiesHost<double>& prop_h);
}



