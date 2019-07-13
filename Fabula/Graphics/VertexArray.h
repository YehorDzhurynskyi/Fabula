#pragma once

#include "Fabula/Graphics/Buffer/VertexBuffer.h"

namespace fbl
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Init();

    fblBool IsValid() const;

    void Bind();
    void Unbind();

    template<BufferUsage Usage>
    void BindVertexBuffer(VertexBuffer<Usage>& buffer)
    {
        Bind();
        buffer.BindLayout();
    }

    template<BufferUsage Usage>
    void BindIndexBuffer(IndexBuffer<Usage>& buffer)
    {
        Bind();
        buffer.Bind();
    }

protected:
    fblVertexArrayID m_VAO;
};

}
