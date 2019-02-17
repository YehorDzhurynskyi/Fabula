#include "pch.h"
#include "Node.h"

Node::Node(Layer* masterLayer)
    : m_masterLayer(masterLayer)
{
    assert(m_masterLayer != nullptr);
}

