#pragma once

#include "DynamicBuffer.h"
#include "Graphics/API/opengl.h"

template<typename T, size_t Capacity>
class DynamicVertexBuffer : public DynamicBuffer<T, Capacity>
{
public:
    DynamicVertexBuffer()
    {
        FBL_GL_CALL(glGenBuffers(1, &m_VBO));
        FBL_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        FBL_GL_CALL(glBufferData(GL_ARRAY_BUFFER, Capacity * sizeof(T), nullptr, GL_DYNAMIC_DRAW));
    }

    ~DynamicVertexBuffer()
    {
        FBL_GL_CALL(glDeleteBuffers(1, &m_VBO));
    }

    void bind()
    {
        FBL_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    }

    void flush()
    {
        FBL_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        FBL_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, this->m_size * sizeof(T), (void*)this->m_data));

        this->m_size = 0;
    }

    VertexBufferID getVertexBufferID() const
    {
        return m_VBO;
    }

private:
    VertexBufferID m_VBO;
};
