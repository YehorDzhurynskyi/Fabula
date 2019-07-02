#pragma once

#include "DynamicBuffer.h"

#include "Fabula/Graphics/API/opengl.h"

namespace fbl
{

template<typename T, size_t Capacity>
class DynamicVertexBuffer : public DynamicBuffer<T, Capacity>
{
public:
    DynamicVertexBuffer()
    {
        fblGLCall(glGenBuffers(1, &m_VBO));
        fblGLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        fblGLCall(glBufferData(GL_ARRAY_BUFFER, Capacity * sizeof(T), nullptr, GL_DYNAMIC_DRAW));
    }

    ~DynamicVertexBuffer()
    {
        fblGLCall(glDeleteBuffers(1, &m_VBO));
    }

    void bind()
    {
        fblGLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    }

    void flush()
    {
        fblGLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        fblGLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, this->m_size * sizeof(T), (void*)this->m_data));

        this->m_size = 0;
    }

    VertexBufferID getVertexBufferID() const
    {
        return m_VBO;
    }

private:
    VertexBufferID m_VBO;
};

}
