#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

struct s_EGL	g_EGL = { 0 };

GFAPII bool	gf_int_initPlatformEGL(void) {
	const char	*_names[] = { "libEGL.so", "libEGL.so.1", "libEGL.so.1.1.0", 0 };
	
	/* Check if the handle isn't already loaded
	 * */
	if (g_EGL.handle) {
		return (true);
	}
	/* Open the libX11 shared library
	 * */
	for (size_t i = 0; !g_EGL.handle && _names[i]; i++) {
		g_EGL.handle = dlopen(_names[i], RTLD_NOW | RTLD_GLOBAL);
	}
	if (!g_EGL.handle) {

#if defined (VERBOSE)
		gf_loge("PLATFORM: Failed to load EGL library\n");
#endif

		return (false);
	}
	
	/* Load all the necessary EGL functions
	 * */
	if (!(g_EGL.eglGetDisplay = dlsym(g_EGL.handle, "eglGetDisplay"))) return (false);
	if (!(g_EGL.eglInitialize = dlsym(g_EGL.handle, "eglInitialize"))) return (false);
	if (!(g_EGL.eglMakeCurrent = dlsym(g_EGL.handle, "eglMakeCurrent"))) return (false);
	if (!(g_EGL.eglBindAPI = dlsym(g_EGL.handle, "eglBindAPI"))) return (false);
	if (!(g_EGL.eglChooseConfig = dlsym(g_EGL.handle, "eglChooseConfig"))) return (false);
	if (!(g_EGL.eglCreateWindowSurface = dlsym(g_EGL.handle, "eglCreateWindowSurface"))) return (false);
	if (!(g_EGL.eglCreateContext = dlsym(g_EGL.handle, "eglCreateContext"))) return (false);
	if (!(g_EGL.eglDestroyContext = dlsym(g_EGL.handle, "eglDestroyContext"))) return (false);
	if (!(g_EGL.eglTerminate = dlsym(g_EGL.handle, "eglTerminate"))) return (false);
	if (!(g_EGL.eglDestroySurface = dlsym(g_EGL.handle, "eglDestroySurface"))) return (false);
	if (!(g_EGL.eglSwapBuffers = dlsym(g_EGL.handle, "eglSwapBuffers"))) return (false);
	if (!(g_EGL.eglSwapInterval = dlsym(g_EGL.handle, "eglSwapInterval"))) return (false);
	return (true);
}
