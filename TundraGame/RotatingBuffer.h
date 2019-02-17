#pragma once

#include <iterator>
#include "types.h"
#include "common.h"

namespace
{
    i32 rot_prev(const i32 pos, const size_t size)
    {
        return (pos + size - 1) % size;
    }

    i32 rot_next(const i32 pos, const size_t size)
    {
        return (pos + 1) % size;
    }
}

template<typename Type>
class RotatingIterator final
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Type;
    using difference_type = std::ptrdiff_t;
    using pointer = Type*;
    using reference = Type&;

    RotatingIterator(const i32 position, const i32 cursor, Type* dataOrigin, const size_t size)
    : m_position(position)
    , m_cursor(cursor)
    , m_dataOrigin(dataOrigin)
    , m_size(size)
    {
        assert(isValid());
    }

    RotatingIterator(const RotatingIterator& rhs) = default;
    RotatingIterator(RotatingIterator&& rhs) = default;
    RotatingIterator& operator=(const RotatingIterator& rhs) = default;
    RotatingIterator& operator=(RotatingIterator&& rhs) = default;
    ~RotatingIterator() = default;

    bool isValid() const
    {
        return
            m_position >= 0 && m_position < m_size &&
            m_dataOrigin != nullptr &&
            m_cursor >= 0 && m_cursor <= m_size;
    }

    void swap(RotatingIterator& rhs)
    {
        std::swap(m_position, rhs.m_position);
        std::swap(m_cursor, rhs.m_cursor);
        std::swap(m_dataOrigin, rhs.m_dataOrigin);
        std::swap(m_size, rhs.m_size);
    }

    RotatingIterator& operator++()
    {
        next();
        return *this;
    }

    RotatingIterator operator++(int)
    {
        RotatingIterator tmp(*this);
        next();
        return tmp;
    }

    RotatingIterator& operator--()
    {
        prev();
        return *this;
    }

    RotatingIterator operator--(int)
    {
        RotatingIterator tmp(*this);
        prev();
        return tmp;
    }

    bool operator==(const RotatingIterator<Type>& rhs) const
    {
        return
            m_position == rhs.m_position &&
            m_cursor == rhs.m_cursor &&
            m_dataOrigin == rhs.m_dataOrigin &&
            m_size == rhs.m_size;
    }

    bool operator!=(const RotatingIterator<Type>& rhs) const
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

    operator RotatingIterator<const Type>() const
    {
        return RotatingIterator<const Type>(m_position, m_cursor, m_dataOrigin, m_size);
    }

private:
    void next()
    {
        assert(isValid());
        m_position = rot_next(m_position, m_size);
        --m_cursor;
    }

    void prev()
    {
        assert(isValid());
        m_position = rot_prev(m_position, m_size);
        ++m_cursor;
    }

private:
    i32 m_position;
    i32 m_cursor;
    Type* m_dataOrigin;
    const size_t m_size;
};

template<typename Type, size_t Capacity>
class RotatingBuffer
{
public:
    using iterator = RotatingIterator<Type>;
    using const_iterator = RotatingIterator<const Type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    RotatingBuffer()
    : m_position(-1)
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
        m_position = rot_next(m_position, Capacity);
        m_data[m_position] = item;
        return _push();
    }

    Type& push(Type&& item = Type())
    {
        m_position = rot_next(m_position, Capacity);
        m_data[m_position] = std::move(item);
        return _push();
    }

    iterator begin()
    {
        return m_size > 0 ?
            iterator(rot_next(m_position, m_size), m_size, m_data, m_size) :
            end();
    }

    iterator end()
    {
        return iterator(m_position, 0, m_data, m_size);
    }

    const_iterator begin() const
    {
        return m_size > 0 ?
            const_iterator(rot_next(m_position, m_size), m_size, m_data, m_size) :
            end();
    }

    const_iterator end() const
    {
        return const_iterator(m_position, 0, m_data, m_size);
    }

    reverse_iterator rbegin()
    {
        return m_size > 0 ?
            reverse_iterator(end()) :
            rend();
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const
    {
        return m_size > 0 ?
            const_reverse_iterator(end()) :
            rend();
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

private:
    Type& _push()
    {
        Type& result = m_data[m_position];
        m_size = std::min<size_t>(m_size + 1, Capacity);
        return result;
    }

private:
    i32 m_position;
    size_t m_size;
    Type m_data[Capacity];
};
