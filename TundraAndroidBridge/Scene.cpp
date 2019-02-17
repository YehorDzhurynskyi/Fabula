#include "pch.h"
#include "Scene.h"

bool Scene::isActive() const
{
    return m_isActive;
}

void Scene::show()
{
    assert(!m_isActive);

    m_isActive = true;
    if (m_onShowCallback)
    {
        m_onShowCallback(this);
    }
}

void Scene::hide()
{
    assert(m_isActive);

    m_isActive = false;
    if (m_onHideCallback)
    {
        m_onHideCallback(this);
    }
}

void Scene::setOnShowCallback(std::function<void(Scene*)> callback)
{
    m_onShowCallback = callback;
}

void Scene::setOnHideCallback(std::function<void(Scene*)> callback)
{
    m_onHideCallback = callback;
}
