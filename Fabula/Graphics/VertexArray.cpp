#include "pch.h"
#include "VertexArray.h"

namespace fbl
{

VertexArray::VertexArray()
    : m_VAO(0)
{
    fblGLCall(glGenVertexArrays(1, &m_VAO));
}

VertexArray::~VertexArray()
{
    fblGLCall(glDeleteVertexArrays(1, &m_VAO));
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
