#pragma once

#include "types.h"

template<typename T, size_t Capacity>
class DynamicBuffer
{
public:
    DynamicBuffer() = default;
    DynamicBuffer(const DynamicBuffer& rhs) = delete;
    DynamicBuffer& operator=(const DynamicBuffer& rhs) = delete;
    DynamicBuffer(DynamicBuffer&& rhs) = delete;
    DynamicBuffer& operator=(DynamicBuffer&& rhs) = delete;
    ~DynamicBuffer() = default;

    T& push(T&& value = T())
    {
        assert(m_size < Capacity);
        m_data[m_size] = std::move(value);
        return m_data[m_size++];
    }

    size_t capacity() const
    {
        return Capacity;
    }

    size_t size() const
    {
        return m_size;
    }

    const T& operator[](const i32 idx)
    {
        assert(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

    const T& operator[](const i32 idx) const
    {
        assert(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

protected:
    T m_data[Capacity];
    size_t m_size = 0;
};
