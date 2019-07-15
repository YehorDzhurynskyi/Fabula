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

template<BufferTarget Target, BufferUsage Usage>
class Buffer
{
public:
    explicit Buffer(fblSize_t capacity)
        : m_Capacity(capacity)
        , m_Size(0)
        , m_BufferID(0)
        , m_MappedData(nullptr)
        , m_Offset(0)
        , m_Bounded(false)
    {}

    ~Buffer()
    {
        if (IsValid())
        {
            fblGLCall(glDeleteBuffers(1, &m_BufferID));
        }
    }

    Buffer(const Buffer& rhs) = delete;
    Buffer& operator=(const Buffer& rhs) = delete;
    Buffer(Buffer&& rhs) = delete;
    Buffer& operator=(Buffer&& rhs) = delete;

    fblBool IsValid() const
    {
        return m_BufferID != 0;
    }

    void Init()
    {
        fblGLCall(glGenBuffers(1, &m_BufferID));
        fblGLCall(glBindBuffer((GLenum)Target, m_BufferID));
        fblGLCall(glBufferData((GLenum)Target, m_Capacity, nullptr, (GLenum)Usage));
    }

    void Bind()
    {
        if (!m_Bounded)
        {
            fblGLCall(glBindBuffer((GLenum)Target, m_BufferID));
            m_Bounded = true;
        }
    }

    void Unbind()
    {
        fblAssert(m_Bounded, "Object should have been bounded before");

        m_Bounded = false;

        fblGLCall(glBindBuffer((GLenum)Target, 0));
    }

    fblSize_t GetCapacity() const
    {
        return m_Capacity;
    }

    fblSize_t GetSize() const
    {
        return m_Size;
    }

    template<typename T>
    void Push(T&& value = T())
    {
        fblAssert(m_Offset < this->m_Capacity, "Out of bounds");

        Bind();

        if (!IsMapped())
        {
            Map();
        }

        memcpy(m_MappedData + m_Offset, &value, sizeof(T));
        m_Offset += sizeof(T);
        ++m_Size;
    }

    void Flush()
    {
        assert(IsMapped());

        Bind();

        if (IsMapped())
        {
            Unmap();
        }
    }

protected:
    void Map()
    {
        Bind();

        // TODO: handle access mode in some way
        m_MappedData = (fblU8*)fblGLCall(glMapBuffer((GLenum)Target, GL_READ_WRITE));
        fblAssert(m_MappedData != nullptr, "Error on buffer object mapping");
    }

    void Unmap()
    {
        Bind();

        const fblBool status = fblGLCall(glUnmapBuffer((GLenum)Target));
        fblAssert(status, "Error on buffer object unmapping");

        m_Size = 0;
        m_Offset = 0;
        m_MappedData = nullptr;
    }

    fblBool IsMapped() const
    {
        return m_MappedData != nullptr;
    }

protected:
    const fblSize_t m_Capacity;
    fblSize_t m_Size;
    fblBufferID m_BufferID;

private:
    fblU8* m_MappedData;
    fblU32 m_Offset;
    fblBool m_Bounded;
};

template<BufferUsage Usage>
using IndexBuffer = Buffer<BufferTarget::IndexBuffer, Usage>;

}
