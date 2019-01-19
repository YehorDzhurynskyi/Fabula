#include "pch.h"
#include "Game/MotionBlurRenderPass.h"

namespace
{

const char* g_MotionBlurVertexShaderSource = ""
"attribute vec2 a_position;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_Position = vec4(a_position, 0.0, 1.0);\n"
"}\n";

const char* g_MotionBlurFragmentShaderSource = ""
#ifdef FBL_WIN32
"#version 140\n"
#endif
"precision mediump float;\n"
"\n"
"uniform vec2 u_prevCameraPos;\n"
"uniform vec2 u_currentCameraPos;\n"
"uniform sampler2D u_inTexture;\n"
"\n"
"void main(void)\n"
"{\n"
"    vec2 blurVec = u_currentCameraPos - u_prevCameraPos;\n"
"    gl_FragColor = texture2D(u_inTexture, gl_Position.xy);\n"
"    for (int i = 1; i < 32; ++i)\n"
"    {\n"
"        vec2 offset = blurVec * (float(i) / float(32 - 1) - 0.5);\n"
"        gl_FragColor += texture2D(u_inTexture, gl_Position.xy + offset);\n"
"    }\n"
"    gl_FragColor /= float(32);\n"
"}\n";

}
