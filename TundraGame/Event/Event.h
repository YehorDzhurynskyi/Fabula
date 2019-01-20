#pragma once

#define EmitEventRTTI(_type)    \
                                \
virtual EventType type() const  \
{                               \
    return EventType::_type;    \
}                               \
                                \


enum class EventType : u8
{
    None = 0
};

struct Event
{
    EmitEventRTTI(None)
};
