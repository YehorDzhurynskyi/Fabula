#include "pch.h"
#include "VertexArray.h"

namespace fbl
{

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_VAO);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}
	
void VertexArray::Bind()
{
	glBindVertexArray(m_VAO);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::BindVertexBuffer(VertexBuffer& buffer)
{
	glBindVertexArray(m_VAO);
	buffer.Bind();
}

void VertexArray::BindIndexBuffer(IndexBuffer& buffer)
{
	glBindVertexArray(m_VAO);
	buffer.Bind();
}

}
