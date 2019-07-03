#pragma once

#include "Buffer.h"

namespace fbl
{

template<typename T, size_t Capacity>
class IndexBuffer : public Buffer<T, Capacity>
{
public:
    IndexBuffer(const BufferUsage usage)
    {
        fblGLCall(glGenBuffers(1, &m_IBO));
        fblGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
        fblGLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Capacity * sizeof(T), nullptr, (GLenum)usage));
    }

    IndexBuffer(const IndexBuffer& rhs) = delete;
    IndexBuffer& operator=(const IndexBuffer& rhs) = delete;
    IndexBuffer(IndexBuffer&& rhs) = delete;
    IndexBuffer& operator=(IndexBuffer&& rhs) = delete;

    ~IndexBuffer()
    {
        fblGLCall(glDeleteBuffers(1, &m_IBO));
    }

    void Bind()
    {
        fblGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
    }

    void Flush()
    {
        fblGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
        fblGLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->m_Size * sizeof(T), (void*)this->m_Data));

        this->m_Size = 0;
    }

    fblIndexBufferID GetIndexBufferID() const
    {
        return m_IBO;
    }

private:
    fblIndexBufferID m_IBO;
};

}
