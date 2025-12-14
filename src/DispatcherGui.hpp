#pragma once

#include "EventGui.hpp"

#include <vector>

namespace gui
{
	class Dispatcher
	{
	public:

		void addObserverGui(Observer& observer_gui);
		void removeObserverGui(Observer& observer_gui);

		void notifyObservers(Event& event);

	private:

		std::vector<Observer*> m_observers;

	};
}