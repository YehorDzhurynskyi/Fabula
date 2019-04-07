#pragma once

#define EmitEventRTTI(_type)                \
                                            \
_type()                                     \
{                                           \
    m_TypeID = get_event_type_id<_type>();  \
}                                           \
                                            \
static EventTypeID TypeID()                 \
{                                           \
    return get_event_type_id<_type>();      \
}                                           \
                                            \
const char* GetName() const override        \
{                                           \
    return #_type;                          \
}                                           \
                                            \

using EventTypeID = i32;
extern EventTypeID EventTypeIDNone;
extern EventTypeID g_TypeIDCounter;

template<typename T>
EventTypeID get_event_type_id()
{
    static EventTypeID typeId = ++g_TypeIDCounter;
    return typeId;
}

struct IEvent
{
public:
    virtual ~IEvent() = default;
    virtual const char* GetName() const = 0;
    EventTypeID GetEventTypeID() const;

protected:
    EventTypeID m_TypeID;
};

class Layer;
using EventHandler = std::function<bool(const IEvent& event)>;
class EventListener final
{
public:
    EventListener();
    ~EventListener();
    EventListener(const EventListener& rhs) = delete;
    EventListener& operator=(const EventListener& rhs) = delete;
    EventListener(EventListener&& rhs) = delete;
    EventListener& operator=(EventListener&& rhs) = delete;

    bool isValid() const;
    bool handle(const IEvent& event) const;

    void on(EventTypeID eventType, EventHandler handler);
    void bind(Layer& layer);
    void unbind();

private:
    Layer* m_masterLayer;
    EventTypeID m_eventType;
    EventHandler m_handler;
};
