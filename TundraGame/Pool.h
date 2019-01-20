#pragma once

#include <array>

template<typename T, size_t N>
class Pool
{
public:
    struct Node
    {
        T Value;
        Node* Next;
        bool InUse;
    };

public:
    Pool()
    {
        m_firstAvailableNode = &m_nodes[0];

        FOR(m_nodes.size() - 1)
        {
            m_nodes[index].Next = &m_nodes[index + 1];
            m_nodes[index].InUse = false;
        }
        m_nodes[m_nodes.size() - 1].Next = nullptr;
        m_nodes[m_nodes.size() - 1].InUse = false;
    }

    template<typename ...Args>
    T* push(Args&& ...args)
    {
        assert(m_firstAvailableNode != nullptr);

        Node* occupiedNode = m_firstAvailableNode;
        m_firstAvailableNode = occupiedNode->Next;

#ifdef _DEBUG
        ++Count;
#endif

        occupiedNode->InUse = true;
        return new (&occupiedNode->Value) T(std::forward<Args>(args)...);
    }

    void rescan()
    {
        for (Node& node : m_nodes)
        {
            if (node.InUse && !node.Value.isAlive())
            {
                node.Next = m_firstAvailableNode;
                node.InUse = false;
                m_firstAvailableNode = &node;

#ifdef _DEBUG
                --Count;
                memset(&node.Value, 0xfb, sizeof(T));
#endif
            }
        }
    }

    auto begin() -> typename std::array<Node, N>::iterator
    {
        return m_nodes.begin();
    }

    auto end() -> typename std::array<Node, N>::iterator
    {
        return m_nodes.end();
    }

public:
#ifdef _DEBUG
    i32 Count = 0;
#endif

private:
    std::array<Node, N> m_nodes;
    Node* m_firstAvailableNode;
};

