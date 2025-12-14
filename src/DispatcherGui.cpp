#include "DispatcherGui.hpp"

namespace gui
{
	void Dispatcher::addObserverGui(Observer& observer_gui)
	{
		m_observers.push_back(&observer_gui);
	}

	void Dispatcher::removeObserverGui(Observer& observer_gui)
	{
		for (auto iter = m_observers.begin(); iter != m_observers.end(); ++iter)
			if (*iter == &observer_gui)
			{
				m_observers.erase(iter);
				break;
			}
	}

	void Dispatcher::notifyObservers(Event& event)
	{
		if(!m_observers.empty())
		{
			for(size_t i = 0;i < m_observers.size();i++)
				m_observers[i]->update(event);
		}
	}
}