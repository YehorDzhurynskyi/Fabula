#pragma once

#include <array>

template<typename T, size_t N>
class Pool
{
public:
    struct Node
    {
        T Value;
        Node* Next = nullptr;
        Node* Prev = nullptr;
        bool IsFree = true;

        void free()
        {
            IsFree = true;
            Prev->Next = this;
        }
    };

public:
    Pool()
    {
        Node* prevNode = nullptr;
        for (Node& node : m_data)
        {
            if (prevNode != nullptr)
            {
                prevNode->Next = &node;
            }
            prevNode = &node;
        }
    }

    template<typename ...Args>
    T* push(Args&& ...args)
    {
        Node* prevNode = nullptr;
        Node* currentNode = &m_data[0];
        while (!currentNode->IsFree)
        {
            prevNode = currentNode;
            currentNode = currentNode->Next;
        }

        assert(currentNode != nullptr);

        if (prevNode != nullptr)
        {
            prevNode->Next = currentNode->Next;
        }

        currentNode->IsFree = false;
        return new (&currentNode->Value) T(std::forward<Args>(args)...);
    }

private:
    std::array<Node, N> m_data;
};

