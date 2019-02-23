#pragma once

#ifdef WIN32
# define FBL_WIN32
#else
# define FBL_ANDROID
#endif

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif
