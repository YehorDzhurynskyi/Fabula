#pragma once

#include <functional>

#define EmitEventRTTI(_type)    \
                                \
virtual EventType type() const  \
{                               \
    return EventType::_type;    \
}                               \
                                \
virtual const char* name() const\
{                               \
    return #_type;              \
}                               \
                                \

enum class EventType : u8
{
    None = 0,
    Click,
    WindowFocus,
    WindowResized
};

struct Event
{
    EmitEventRTTI(None)

public:
    virtual ~Event() = default;
};

struct ClickEvent : public Event
{
    EmitEventRTTI(Click)

public:
    float NDCXPos;
    float NDCYPos;
};

struct WindowFocusEvent : public Event
{
    EmitEventRTTI(WindowFocus)

public:
    bool Focused;
};

struct WindowResizedEvent : public Event
{
    EmitEventRTTI(WindowResized)

public:
    i32 Width;
    i32 Height;
};

class Layer;
using EventHandler = std::function<void(const Event& event)>;
class EventListener
{
public:
    EventListener();
    ~EventListener();
    EventListener(const EventListener& rhs) = delete;
    EventListener& operator=(const EventListener& rhs) = delete;
    EventListener(EventListener&& rhs) = delete;
    EventListener& operator=(EventListener&& rhs) = delete;

    bool isValid() const;
#if 0
    void reset();
#endif

    void on(EventType eventType, EventHandler handler);
    void bind(Layer& layer);
    void unbind();

#if 0
    bool operator==(const EventListener& rhs) const;
#endif
    void operator()(const Event& event) const;

private:
    Layer* m_masterLayer;
    EventType m_eventType;
    EventHandler m_handler;
};
