#pragma once

#include "Event.hpp"

namespace core
{
	// interface that must be implemented in order
	// to be added in core app
	class ILayer
	{
	public:

		virtual ~ILayer() {}

		virtual void event(Event& event) = 0;
		virtual void update() = 0;
		virtual void draw() = 0;

		virtual size_t getID() = 0;

	};
}