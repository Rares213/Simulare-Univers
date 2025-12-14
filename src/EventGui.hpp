#pragma once

#include "PlatformEnum.hpp"

#include <memory>

namespace gui
{
	enum class EventType 
	{ 
		EventCycleUp,
		EventCycleDown,
		EventCreateContext, 
		EventDeleteContext, 
		EventGravity, 
		EventSoft, 
		EventWorkersCount, 
		EventStepsize, 
		EventNumberBodies,
		EventResetBodies,
		EventForcefinder,
		EventIntegrator,
		EventShapeSize,
	};

	// interface for events made by the gui menu
	class Event
	{
	public:
		virtual ~Event() {}

		virtual EventType getType() = 0;
		virtual const char* getDescriptionEvent() = 0;
	};

	// interface for classes that need to receive events from gui menu
	class Observer
	{
	public:
		virtual ~Observer() {}

		virtual void update(Event& event) = 0;
	};

	class EventCycleUp : public Event
	{
	public:
		EventCycleUp() {}
		virtual ~EventCycleUp() {}

		virtual EventType getType() override { return EventType::EventCycleUp; };
		virtual const char* getDescriptionEvent() override { return "EventCycleUp"; };
	};

	class EventCycleDown : public Event
	{
	public:
		EventCycleDown() {}
		virtual ~EventCycleDown() {}

		virtual EventType getType() override { return EventType::EventCycleDown; };
		virtual const char* getDescriptionEvent() override { return "EventCycleDown"; };
	};

	class EventCreateContext : public Event
	{
	public:
		EventCreateContext(platform::PlatformOption platform_option, platform::PlatformDataType platform_data_type)
			:
			m_platform_option(platform_option),
			m_platform_data_type(platform_data_type)
		{
		}
		virtual ~EventCreateContext() {}

		virtual EventType getType() override { return EventType::EventCreateContext; };
		virtual const char* getDescriptionEvent() override { return "EventCreateContext"; };

		platform::PlatformOption getPlatformOption() { return m_platform_option; }
		platform::PlatformDataType getPlatformDataType() { return m_platform_data_type; }

	private:
		platform::PlatformOption m_platform_option;
		platform::PlatformDataType m_platform_data_type;
	};

	class EventDeleteContext : public Event
	{
	public:
		EventDeleteContext() {}
		virtual ~EventDeleteContext() {}

		virtual EventType getType() override { return EventType::EventDeleteContext; };
		virtual const char* getDescriptionEvent() override { return "EventDeleteContext"; };
	};

	class EventGravity : public Event
	{
	public:
		EventGravity() {}
		virtual ~EventGravity() {}

		virtual EventType getType() override { return EventType::EventGravity; }
		virtual const char* getDescriptionEvent() override { return "EventGravity"; }
	};

	class EventSoft : public Event
	{
	public:
		EventSoft() {}
		virtual ~EventSoft() {}

		virtual EventType getType() override { return EventType::EventSoft; }
		virtual const char* getDescriptionEvent() override { return "EventSoft"; }
	};

	class EventWorkersCount : public Event
	{
	public:
		EventWorkersCount() {}
		virtual ~EventWorkersCount() {}

		virtual EventType getType() override { return EventType::EventWorkersCount; }
		virtual const char* getDescriptionEvent() override { return "EventWorkersCount"; }
	};

	class EventStepsize : public Event
	{
	public:
		EventStepsize() {}
		virtual ~EventStepsize() {}

		virtual EventType getType() override { return EventType::EventStepsize; }
		virtual const char* getDescriptionEvent() override { return "EventStepsize"; }
	};

	class EventNumberBodies : public Event
	{
	public:
		EventNumberBodies() {}
		virtual ~EventNumberBodies() {}

		virtual EventType getType() override { return EventType::EventNumberBodies; }
		virtual const char* getDescriptionEvent() override { return "EventNumberBodies"; }
	};

	class EventResetBodies : public Event
	{
	public:
		EventResetBodies() {}
		virtual ~EventResetBodies() {}

		virtual EventType getType() override { return EventType::EventResetBodies; }
		virtual const char* getDescriptionEvent() override { return "EventResetBodies"; };
	};

	class EventForcefinder : public Event
	{
	public:
		EventForcefinder() {}
		virtual ~EventForcefinder() {}

		virtual EventType getType() override { return EventType::EventForcefinder; }
		virtual const char* getDescriptionEvent() override { return "EventForcefinder"; }
	};

	class EventIntegrator : public Event
	{
	public:
		EventIntegrator() {}
		virtual ~EventIntegrator() {}

		virtual EventType getType() override { return EventType::EventIntegrator; }
		virtual const char* getDescriptionEvent() override { return "EventIntegrator"; }
	};

	class EventShapeSize : public Event
	{
	public:
		EventShapeSize(float size) : m_size(size) {}
		virtual ~EventShapeSize() {}

		virtual EventType getType() override { return EventType::EventShapeSize; }
		virtual const char* getDescriptionEvent() override { return "EventShapeSize"; }

		float getShapeSize() { return m_size; }

	private:

		float m_size;
	};


}