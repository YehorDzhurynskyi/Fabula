#pragma once

#include <array>

template<typename T, size_t N, typename Predicate>
class Pool
{
public:
    struct Node
    {
        T Value;
        Node* Next;
    };

public:
    Pool()
    {
        m_firstAvailableNode = &m_nodes[0];

        FOR(m_nodes.size() - 1)
        {
            m_nodes[index].Next = &m_nodes[index + 1];
        }
        m_nodes[m_nodes.size() - 1].Next = nullptr;
    }

    template<typename ...Args>
    T* push(Args&& ...args)
    {
        assert(m_firstAvailableNode != nullptr);

        Node* occupiedNode = m_firstAvailableNode;
        m_firstAvailableNode = occupiedNode->Next;

        return new (&occupiedNode->Value) T(std::forward<Args>(args)...);
    }

    void rescan()
    {
        for (Node& node : m_nodes)
        {
            if (m_predicate(node.Value))
            {
                node.Next = m_firstAvailableNode;
                m_firstAvailableNode = &node;

#ifdef _DEBUG
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

private:
    Predicate m_predicate;
    std::array<Node, N> m_nodes;
    Node* m_firstAvailableNode;
};

