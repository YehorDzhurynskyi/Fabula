#pragma once

#include "types.h"
#include "common.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

template<typename T, size_t Capacity>
class DynamicBuffer
{
public:
    DynamicBuffer()
        : m_size(0)
    {
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, Capacity * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
    }

    DynamicBuffer(const DynamicBuffer& rhs) = delete;
    DynamicBuffer& operator=(const DynamicBuffer& rhs) = delete;
    DynamicBuffer(DynamicBuffer&& rhs) = delete;
    DynamicBuffer& operator=(DynamicBuffer&& rhs) = delete;

    ~DynamicBuffer()
    {
        glDeleteBuffers(1, &m_VBO);
    }

    void bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    }

    T& push(T&& value = T())
    {
        assert(m_size < Capacity);
        m_data[m_size] = std::move(value);
        return m_data[m_size++];
    }

    void flush()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_size * sizeof(T), (void*)m_data);

        m_size = 0;
    }

    size_t capacity() const
    {
        return Capacity;
    }

    size_t size() const
    {
        return m_size;
    }

    VertexBufferID getVertexBufferID()
    {
        return m_VBO;
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

private:
    T m_data[Capacity];
    size_t m_size;

    VertexBufferID m_VBO;
};
