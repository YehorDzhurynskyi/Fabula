#include "Fabula/pch.h"
#include "VertexArray.h"

namespace fbl
{

VertexArray::VertexArray()
    : m_VAO(0)
{}

VertexArray::~VertexArray()
{
    if (IsValid())
    {
        fblGLCall(glDeleteVertexArrays(1, &m_VAO));
    }
}

fblBool VertexArray::IsValid() const
{
    return m_VAO != 0;
}

void VertexArray::Init()
{
    fblGLCall(glGenVertexArrays(1, &m_VAO));
}

void VertexArray::Bind()
{
    fblGLCall(glBindVertexArray(m_VAO));
}

void VertexArray::Unbind()
{
    fblGLCall(glBindVertexArray(0));
}

}
