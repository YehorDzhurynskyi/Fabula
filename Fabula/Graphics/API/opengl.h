#pragma once

#include "glad.h"

#ifdef FBL_CONFIG_DEBUG
# define FBL_GL_CALL(_x) (glad_debug_##_x)
#else
# define FBL_GL_CALL(_x) (glad_##_x)
#endif

void fbl_init_opengl();
