#pragma once

#include "Fabula/types.h"
#include "Fabula/Graphics/types.h"
#include "Fabula/Graphics/API/opengl.h"

namespace fbl
{

enum class BufferTarget : fblU32
{
    VertexBuffer = GL_ARRAY_BUFFER,
    IndexBuffer = GL_ELEMENT_ARRAY_BUFFER
};

enum class BufferUsage : fblU32
{
    StaticRead = GL_STATIC_READ,
    StaticDraw = GL_STATIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicDraw = GL_DYNAMIC_DRAW
};

template<typename T, size_t Capacity, BufferTarget Target, BufferUsage Usage>
class Buffer
{
public:
    Buffer()
	{
        fblGLCall(glGenBuffers(1, &m_BufferID));
        fblGLCall(glBindBuffer(Target, m_BufferID));
        fblGLCall(glBufferData(Target, Capacity * sizeof(T), nullptr, (GLenum)Usage));
	}

    ~Buffer()
	{
		fblGLCall(glDeleteBuffers(1, &m_BufferID));
	}

    Buffer(const Buffer& rhs) = delete;
    Buffer& operator=(const Buffer& rhs) = delete;
    Buffer(Buffer&& rhs) = delete;
    Buffer& operator=(Buffer&& rhs) = delete;

	void Bind()
	{
		if (!m_Bounded)
		{
			fblGLCall(glBindBuffer(Target, m_BufferID));
			m_Bounded = true;
		}
	}
	
	void Unbind()
	{
		m_Bounded = false;
		fblGLCall(glBindBuffer(Target, 0));
	}
	
    size_t GetCapacity() const
    {
        return Capacity;
    }

protected:
	fblBufferID m_BufferID;

private:
	fblBool m_Bounded;
};

template<typename T, size_t Capacity, BufferTarget Target, BufferUsage Usage>
class StaticBuffer : public Buffer<T, Capacity, Target, Usage>
{
public:
	void SubData(fblU32 offset, const void* data, fblSize_t size)
	{
		fblGLCall(glBufferSubData(Target, offset, size, data));
	}
};

template<typename T, size_t Capacity, BufferTarget Target, BufferUsage Usage>
class DynamicBuffer : public Buffer<T, Capacity, Target, Usage>
{
public:
    T& Push(T&& value = T())
    {
        fblAssert(m_Size < Capacity, "Out of bounds");
		
		if (!IsMapped())
		{
			Map();
		}

        m_MappedData[m_Size] = std::move(value);
        return m_MappedData[m_Size++];
    }

	void Flush()
    {
		Unmap();
        m_Size = 0;
    }
	
protected:
	void Map()
	{
		Bind();
		
		// TODO: handle access mode in some way
		m_MappedData = (T*)fblGLCall(glMapBuffer(Target, GL_READ_WRITE));
		fblAssert(m_MappedData != nullptr, "Error on buffer object mapping");
	}

	void Unmap()
	{
		Bind();
		const fblBool status = fblGLcall(glUnmapBuffer(Target));
		fblAssert(status, "Error on buffer object unmapping");

		m_MappedData = nullptr;
	}

	fblBool IsMapped() const
	{
		return m_MappedData != nullptr;
	}

protected:
	T* m_MappedData = nullptr;
	fblSize_t m_Size = 0;
};

    StaticRead = GL_STATIC_READ,
    StaticDraw = GL_STATIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicDraw = GL_DYNAMIC_DRAW

template<typename T, size_t Capacity>
using VertexBufferDynamicDraw = DynamicBuffer<T, Capacity, BufferTarget::VertexBuffer, BufferUsage::DynamicDraw>;
using VertexBufferDynamic = VertexBufferDynamicDraw;

template<typename T, size_t Capacity>
using VertexBufferStaticDraw = DynamicBuffer<T, Capacity, BufferTarget::VertexBuffer, BufferUsage::StaticDraw>;
using VertexBufferStatic = VertexBufferStaticDraw;

}
