#pragma once

#ifdef WIN32
# define FBL_PLATFORM_WIN32
#else
# define FBL_PLATFORM_ANDROID
#endif

#if !defined(NDEBUG) || defined(DEBUG)
# define FBL_CONFIG_DEBUG
#else
# define FBL_CONFIG_RELEASE
#endif

#ifdef FBL_CONFIG_DEBUG
# define FBL_DEBUG_TRIGGER_BREAKPOINT SDL_TriggerBreakpoint
#else
# define FBL_DEBUG_TRIGGER_BREAKPOINT
#endif
