#pragma once

#include "glad.h"

#ifdef _DEBUG
# define fblGLCall(_x) (glad_debug_##_x)
#else
# define fblGLCall(_x) (glad_##_x)
#endif

void fbl_init_opengl();
