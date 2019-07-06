#pragma once

#include "Buffer.h"

namespace fbl
{

template<typename T, size_t Capacity, BufferTarget Target, BufferUsage Usage>
class Buffer : public Buffer<T, Capacity, Target, Usage>
{
public:
    Buffer() = default;
    Buffer(const Buffer& rhs) = delete;
    Buffer& operator=(const Buffer& rhs) = delete;
    Buffer(Buffer&& rhs) = delete;
    Buffer& operator=(Buffer&& rhs) = delete;
    ~Buffer() = default;

    T& Push(T&& value = T())
    {
        assert(m_Size < Capacity);
        m_Data[m_Size] = std::move(value);
        return m_Data[m_Size++];
    }

    size_t GetCapacity() const
    {
        return Capacity;
    }

    size_t GetSize() const
    {
        return m_Size;
    }

    const T& operator[](const fblS32 idx)
    {
        assert(idx >= 0 && idx < m_Size);
        return m_Data[idx];
    }

    const T& operator[](const fblS32 idx) const
    {
        assert(idx >= 0 && idx < m_Size);
        return m_Data[idx];
    }

protected:
    T m_Data[Capacity];
    size_t m_Size = 0;
};

}
