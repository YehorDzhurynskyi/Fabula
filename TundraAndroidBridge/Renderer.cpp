#include "pch.h"
#include "Renderer.h"

#ifdef FBL_ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include "glew.h"
#endif

namespace
{

const char* g_VertexShaderSource = ""
"attribute vec2 a_vertex\n"
"void main(void)\n"
"{\n"
"    gl_Position = a_vertex;\n"
"}\n";

const char* g_FragmentShaderSource = ""
"void main(void)\n"
"{\n"
"    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n";

}

void Renderer::init()
{
    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
}

void Renderer::shutdown()
{

}

void Renderer::render()
{

}
