#pragma once

namespace fbl
{

class Scene
{
public:
    void Bind();
    void Unbind();

protected:
    fblVertexArrayID m_VAO;
};

}
