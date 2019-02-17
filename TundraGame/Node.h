#pragma once

class Layer;
class Node
{
public:
    Node(Layer* masterLayer);

    Layer* getMasterLayer();

private:
    Layer* m_masterLayer;
};
