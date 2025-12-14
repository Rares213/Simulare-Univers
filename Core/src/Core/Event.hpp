#pragma once


namespace core
{
	enum class EventType { KeyPressed, KeyReleased, KeyRepeat, CursorPosition };
	
	// interface for all events issues by core
	// use getType in order to downcast to 
	// proper event type
	class Event
	{
	public:

		virtual ~Event() {}

		virtual EventType getType() = 0;
		virtual const char* getDescription() = 0;

	};
	
	class EventKey : public Event
	{
	public:
		EventKey(int key) : m_key(key) {}
		virtual ~EventKey() {}

		int getKey() { return m_key; }

	protected:

		int m_key;
	};

	class EventKeyPressed : public EventKey
	{
	public:
		EventKeyPressed(int key) : EventKey(key) {}
		virtual ~EventKeyPressed() {}

		virtual EventType getType() override { return EventType::KeyPressed; }
		virtual const char* getDescription() override { return "KeyPressedEvent"; }
	};

	class EventKeyReleased : public EventKey
	{
	public:
		EventKeyReleased(int key) : EventKey(key) {}
		virtual ~EventKeyReleased() {}

		virtual EventType getType() override { return EventType::KeyReleased; }
		virtual const char* getDescription() override { return "KeyReleasedEvent"; }
	};

	class EventKeyRepeat : public EventKey
	{
	public:
		EventKeyRepeat(int key) : EventKey(key) {}
		virtual ~EventKeyRepeat() {}

		virtual EventType getType() override { return EventType::KeyRepeat; }
		virtual const char* getDescription() override { return "KeyRepeatEvent"; }
	};

	class EventCursorPos : public Event
	{
	public:
		EventCursorPos(double xpos, double ypos) : m_xpos(xpos), m_ypos(ypos) {}
		virtual ~EventCursorPos() {}

		virtual EventType getType() override { return EventType::CursorPosition; }
		virtual const char* getDescription() override { return "MousePositionEvent"; }

		double getXpos() { return m_xpos; }
		double getYpos() { return m_ypos; }

	public:
		double m_xpos;
		double m_ypos;
	};

	

}