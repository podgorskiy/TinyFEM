#define _GLFW_USE_OPENGL

#ifdef _WIN32
#define _GLFW_WIN32
#define _GLFW_WGL
#endif

#include <GL/glew.h>

#include "context.c"
#include "init.c"
#include "input.c"
#include "monitor.c"
#include "window.c"

#ifdef _WIN32
#include "wgl_context.c"
#include "win32_init.c"
#include "win32_monitor.c"
#include "win32_time.c"
#include "win32_tls.c"
#include "win32_window.c"
#include "winmm_joystick.c"
#else
#include "mir_init.c"
#include "mir_monitor.c"
#include "mir_window.c"
#include "posix_time.c"
#include "posix_tls.c"
#include "wl_init.c"
#include "wl_monitor.c"
#include "wl_window.c"
#include "x11_init.c"
#include "x11_monitor.c"
#include "x11_window.c"
#include "xkb_unicode.c"
#include "egl_context.c"
#include "glx_context.c"
#include "linux_joystick.c"
#include "mach_time.c"
#endif