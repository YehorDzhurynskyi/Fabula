#include "Fabula/Graphics/Buffer/VertexBuffer.h"
#include "Fabula/Graphics/Buffer/IndexBuffer.h"

namespace fbl
{

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	
	void Bind();
	void Unbind();
	
	void BindVertexBuffer(VertexBuffer& buffer);
	void BindIndexBuffer(IndexBuffer& buffer);

protected:
	fblVertexArrayID m_VAO;
};

}
