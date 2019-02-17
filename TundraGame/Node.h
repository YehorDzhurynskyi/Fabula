#pragma once

class Layer;
class Node
{
public:
    virtual ~Node() = default;

    Layer* getMasterLayer()
    {
        assert(m_masterLayer != nullptr && "Probably this node should be added to some Layer");
        return m_masterLayer;
    }

    void connect(Layer& newLayer)
    {
        if (m_masterLayer != nullptr)
        {
            onDisconnect();
        }

        m_masterLayer = &newLayer;
        onConnect();
    }

protected:
    virtual void onConnect() = 0;
    virtual void onDisconnect() = 0;

private:
    Layer* m_masterLayer = nullptr;
};
