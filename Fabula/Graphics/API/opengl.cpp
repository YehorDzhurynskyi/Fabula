#include "pch.h"
#include "opengl.h"

namespace
{
void glcall_post_callback(const char *name, void *funcptr, int len_args, ...)
{
    (void)funcptr;
    (void)len_args;

    const GLenum errCode = glad_glGetError();
    if (errCode != GL_NO_ERROR)
    {
        char buff[4096];
        sprintf_s(buff, sizeof(buff), "[ERROR][OPENGL][Code %d] in `%s` function\n", errCode, name);
        OutputDebugStringA(buff);
        FBL_DEBUG_TRIGGER_BREAKPOINT();
    }
}
}

void fbl_init_opengl()
{
    const bool gladStatus = gladLoadGL();
    assert(gladStatus);

    glad_set_post_callback(glcall_post_callback);

    FBL_GL_CALL(glEnable(GL_BLEND));
    FBL_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    FBL_GL_CALL(glClearColor(0.98f, 0.98f, 0.98f, 1.0f));
}
