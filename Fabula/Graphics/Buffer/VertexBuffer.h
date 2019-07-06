#pragma once

#include "Buffer.h"

namespace fbl
{

class VertexBufferLayout
{
public:
    struct Attribute
    {
        enum class DataType
        {
            s_Byte, u_Byte, s_Short, u_Short, s_Int, u_Int,
            s_Byte2, u_Byte2, s_Short2, u_Short2, s_Int2, u_Int2,
            s_Byte3, u_Byte3, s_Short3, u_Short3, s_Int3, u_Int3,
            s_Byte4, u_Byte4, s_Short4, u_Short4, s_Int4, u_Int4,
            Float, Float2, Float3, Float4
        };

        DataType Type;
        fblBool Normalized;
        fblU32 Offset;

        static fblU32 CountOfDataType(DataType type);
        static GLenum GLTypeOfDataType(DataType type);
    };

public:
    VertexBufferLayout(std::initializer_list<Attribute>&& attributes)
        : m_Attributes(attributes)
    {}

    std::vector<Attribute>::iterator begin() { return m_Attributes.begin(); }
    std::vector<Attribute>::iterator end() { return m_Attributes.end(); }
    std::vector<Attribute>::const_iterator begin() const { return m_Attributes.begin(); }
    std::vector<Attribute>::const_iterator end() const { return m_Attributes.end(); }

private:
    std::vector<Attribute> m_Attributes;
};

template<BufferUsage Usage>
class VertexBuffer : public Buffer<BufferTarget::VertexBuffer, Usage>
{
public:
    using super = Buffer;

public:
    explicit VertexBuffer(fblSize_t capacity, VertexBufferLayout&& layout)
        : super(capacity)
        , m_Layout(std::move(layout))
    {}

    void BindLayout()
    {
        this->Bind();

        fblShaderLocationID location = 0;
        for (const VertexBufferLayout::Attribute& attr : m_Layout)
        {
            fblGLCall(glEnableVertexAttribArray(location));
            fblGLCall(glVertexAttribPointer(
                location,
                VertexBufferLayout::Attribute::CountOfDataType(attr.Type),
                VertexBufferLayout::Attribute::GLTypeOfDataType(attr.Type),
                attr.Normalized,
                0,
                (void*)attr.Offset
            ));
            ++location;
        }
    }

    void UnbindLayout()
    {
        this->Unbind();

        fblShaderLocationID location = 0;
        for (const VertexBufferLayout::Attribute& attr : m_Layout)
        {
            fblGLCall(glDisableVertexAttribArray(location++));
        }
    }

protected:
    VertexBufferLayout m_Layout;
};

inline fblU32 VertexBufferLayout::Attribute::CountOfDataType(DataType type)
{
    fblU32 count = 0;

    switch (type)
    {
    case DataType::s_Byte:
    case DataType::u_Byte:
    case DataType::s_Short:
    case DataType::u_Short:
    case DataType::s_Int:
    case DataType::u_Int:
    case DataType::Float:
    count = 1; break;
    case DataType::s_Byte2:
    case DataType::u_Byte2:
    case DataType::s_Short2:
    case DataType::u_Short2:
    case DataType::s_Int2:
    case DataType::u_Int2:
    case DataType::Float2:
    count = 2; break;
    case DataType::s_Byte3:
    case DataType::u_Byte3:
    case DataType::s_Short3:
    case DataType::u_Short3:
    case DataType::s_Int3:
    case DataType::u_Int3:
    case DataType::Float3:
    count = 3; break;
    case DataType::s_Byte4:
    case DataType::u_Byte4:
    case DataType::s_Short4:
    case DataType::u_Short4:
    case DataType::s_Int4:
    case DataType::u_Int4:
    case DataType::Float4:
    count = 4; break;
    }

    fblAssert(count != 0, "Unrecognized type");

    return count;
}

inline GLenum VertexBufferLayout::Attribute::GLTypeOfDataType(DataType type)
{
    GLenum glType = GL_NONE;

    switch (type)
    {
    case DataType::u_Byte:
    case DataType::u_Byte2:
    case DataType::u_Byte3:
    case DataType::u_Byte4:
    glType = GL_UNSIGNED_BYTE; break;
    case DataType::s_Byte:
    case DataType::s_Byte2:
    case DataType::s_Byte3:
    case DataType::s_Byte4:
    glType = GL_BYTE; break;
    case DataType::u_Short:
    case DataType::u_Short2:
    case DataType::u_Short3:
    case DataType::u_Short4:
    glType = GL_UNSIGNED_SHORT; break;
    case DataType::s_Short:
    case DataType::s_Short2:
    case DataType::s_Short3:
    case DataType::s_Short4:
    glType = GL_SHORT; break;
    case DataType::u_Int:
    case DataType::u_Int2:
    case DataType::u_Int3:
    case DataType::u_Int4:
    glType = GL_UNSIGNED_INT; break;
    case DataType::s_Int:
    case DataType::s_Int2:
    case DataType::s_Int3:
    case DataType::s_Int4:
    glType = GL_INT; break;
    case DataType::Float:
    case DataType::Float2:
    case DataType::Float3:
    case DataType::Float4:
    glType = GL_FLOAT; break;
    }

    fblAssert(glType != GL_NONE, "Unrecognized type");

    return glType;
}

}
