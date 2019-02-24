#pragma once

#include "DynamicBuffer.h"
#include "Graphics/API/opengl.h"

template<typename T, size_t Capacity>
class DynamicIndexBuffer : public DynamicBuffer<T, Capacity>
{
public:
    DynamicIndexBuffer()
    {
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Capacity * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
    }

    DynamicIndexBuffer(const DynamicIndexBuffer& rhs) = delete;
    DynamicIndexBuffer& operator=(const DynamicIndexBuffer& rhs) = delete;
    DynamicIndexBuffer(DynamicIndexBuffer&& rhs) = delete;
    DynamicIndexBuffer& operator=(DynamicIndexBuffer&& rhs) = delete;

    ~DynamicIndexBuffer()
    {
        glDeleteBuffers(1, &m_IBO);
    }

    void bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    }

    void flush()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->m_size * sizeof(T), (void*)this->m_data);

        this->m_size = 0;
    }

    IndexBufferID getIndexBufferID() const
    {
        return m_IBO;
    }

private:
    IndexBufferID m_IBO;
};
