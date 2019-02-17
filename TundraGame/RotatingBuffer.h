#pragma once

#include <iterator>
#include "types.h"
#include "common.h"

template<typename Type, size_t Capacity>
class RotatingForwardIterator final
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Type;
    using difference_type = std::ptrdiff_t;
    using pointer = Type*;
    using reference = Type&;

    RotatingForwardIterator(const i32 position, const i32 size, Type* dataOrigin)
    : m_position(position)
    , m_itemsLeft(size)
    , m_dataOrigin(dataOrigin)
    {}

    RotatingForwardIterator()
    {
        reset();
    }

    RotatingForwardIterator(const RotatingForwardIterator& rhs) = default;
    RotatingForwardIterator(RotatingForwardIterator&& rhs) = default;
    RotatingForwardIterator& operator=(const RotatingForwardIterator& rhs) = default;
    RotatingForwardIterator& operator=(RotatingForwardIterator&& rhs) = default;
    ~RotatingForwardIterator() = default;

    bool isValid() const
    {
        return
            m_position >= 0 && m_position < Capacity &&
            m_dataOrigin != nullptr &&
            m_itemsLeft > 0 && m_itemsLeft <= Capacity;
    }

    void reset()
    {
        m_position = -1;
        m_itemsLeft = 0;
        m_dataOrigin = nullptr;
    }

    void swap(RotatingForwardIterator& rhs)
    {
        std::swap(m_position, rhs.m_position);
        std::swap(m_dataOrigin, rhs.m_dataOrigin);
    }

    RotatingForwardIterator& operator++()
    {
        next();
        return *this;
    }

    RotatingForwardIterator operator++(int)
    {
        RotatingForwardIterator tmp(*this);
        next();
        return tmp;
    }

    bool operator==(const RotatingForwardIterator<Type, Capacity>& rhs) const
    {
        return
            m_position == rhs.m_position &&
            m_itemsLeft == rhs.m_itemsLeft &&
            m_dataOrigin == rhs.m_dataOrigin;
    }

    bool operator!=(const RotatingForwardIterator<Type, Capacity>& rhs) const
    {
        return !operator==(rhs);
    }

    Type& operator*() const
    {
        assert(isValid());
        return m_dataOrigin[m_position];
    }

    Type& operator->() const
    {
        assert(isValid());
        return m_dataOrigin[m_position];
    }

    operator RotatingForwardIterator<const Type, Capacity>() const
    {
        return RotatingForwardIterator<const Type, Capacity>(m_position, m_itemsLeft, m_dataOrigin);
    }

private:
    void next()
    {
        assert(isValid());
        m_position = (m_position + 1) % Capacity;
        --m_itemsLeft;

        if (m_itemsLeft == 0)
        {
            reset();
        }
    }

private:
    i32 m_position;
    i32 m_itemsLeft;
    Type* m_dataOrigin;
};

template<typename Type, size_t Capacity>
class RotatingBuffer
{
public:
    using iterator = RotatingForwardIterator<Type, Capacity>;
    using const_iterator = RotatingForwardIterator<const Type, Capacity>;

    RotatingBuffer()
    : m_position(0)
    , m_size(0)
    {}

    size_t capacity() const
    {
        return Capacity;
    }

    size_t size() const
    {
        return m_size;
    }

    Type& push(const Type& item)
    {
        m_data[m_position] = item;
        return _push();
    }

    Type& push(Type&& item = Type())
    {
        m_data[m_position] = std::move(item);
        return _push();
    }

    iterator begin()
    {
        return m_size > 0 ?
            iterator(m_position, m_size, m_data) :
            end();
    }

    iterator end()
    {
        return iterator();
    }

    const_iterator begin() const
    {
        return m_size > 0 ?
            const_iterator(m_position, m_size, m_data) :
            end();
    }

    const_iterator end() const
    {
        return const_iterator();
    }

private:
    Type& _push()
    {
        Type& result = m_data[m_position];

        m_position = (m_position + 1) % Capacity;
        m_size = std::min<size_t>(m_size + 1, Capacity);

        return result;
    }

private:
    i32 m_position;
    size_t m_size;
    Type m_data[Capacity];
};
