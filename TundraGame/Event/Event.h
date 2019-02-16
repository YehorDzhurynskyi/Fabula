#pragma once

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