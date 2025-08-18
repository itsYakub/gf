#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

struct s_EGL	g_EGL = { 0 };

GFAPII bool	gf_int_initPlatformEGL(void) {
	const char	*_names[] = { "libEGL.so", "libEGL.so.1", 0 };
	
	/* Check if the handle isn't already loaded
	* */
	if (g_EGL.handle) {
		return (true);
	}
	/* Open the libEGL shared library
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
	if (!(g_EGL.eglBindAPI = dlsym(g_EGL.handle, "eglBindAPI"))) return (false);
	if (!(g_EGL.eglBindTexImage = dlsym(g_EGL.handle, "eglBindTexImage"))) return (false);
	if (!(g_EGL.eglChooseConfig = dlsym(g_EGL.handle, "eglChooseConfig"))) return (false);
	if (!(g_EGL.eglClientWaitSync = dlsym(g_EGL.handle, "eglClientWaitSync"))) return (false);
	if (!(g_EGL.eglCopyBuffers = dlsym(g_EGL.handle, "eglCopyBuffers"))) return (false);
	if (!(g_EGL.eglCreateContext = dlsym(g_EGL.handle, "eglCreateContext"))) return (false);
	if (!(g_EGL.eglCreateImage = dlsym(g_EGL.handle, "eglCreateImage"))) return (false);
	if (!(g_EGL.eglCreatePbufferFromClientBuffer = dlsym(g_EGL.handle, "eglCreatePbufferFromClientBuffer"))) return (false);
	if (!(g_EGL.eglCreatePbufferSurface = dlsym(g_EGL.handle, "eglCreatePbufferSurface"))) return (false);
	if (!(g_EGL.eglCreatePixmapSurface = dlsym(g_EGL.handle, "eglCreatePixmapSurface"))) return (false);
	if (!(g_EGL.eglCreatePlatformPixmapSurface = dlsym(g_EGL.handle, "eglCreatePlatformPixmapSurface"))) return (false);
	if (!(g_EGL.eglCreatePlatformWindowSurface = dlsym(g_EGL.handle, "eglCreatePlatformWindowSurface"))) return (false);
	if (!(g_EGL.eglCreateSync = dlsym(g_EGL.handle, "eglCreateSync"))) return (false);
	if (!(g_EGL.eglCreateWindowSurface = dlsym(g_EGL.handle, "eglCreateWindowSurface"))) return (false);
	if (!(g_EGL.eglDestroyContext = dlsym(g_EGL.handle, "eglDestroyContext"))) return (false);
	if (!(g_EGL.eglDestroyImage = dlsym(g_EGL.handle, "eglDestroyImage"))) return (false);
	if (!(g_EGL.eglDestroySurface = dlsym(g_EGL.handle, "eglDestroySurface"))) return (false);
	if (!(g_EGL.eglDestroySync = dlsym(g_EGL.handle, "eglDestroySync"))) return (false);
	if (!(g_EGL.eglGetConfigAttrib = dlsym(g_EGL.handle, "eglGetConfigAttrib"))) return (false);
	if (!(g_EGL.eglGetConfigs = dlsym(g_EGL.handle, "eglGetConfigs"))) return (false);
	if (!(g_EGL.eglGetCurrentContext = dlsym(g_EGL.handle, "eglGetCurrentContext"))) return (false);
	if (!(g_EGL.eglGetCurrentDisplay = dlsym(g_EGL.handle, "eglGetCurrentDisplay"))) return (false);
	if (!(g_EGL.eglGetCurrentSurface = dlsym(g_EGL.handle, "eglGetCurrentSurface"))) return (false);
	if (!(g_EGL.eglGetDisplay = dlsym(g_EGL.handle, "eglGetDisplay"))) return (false);
	if (!(g_EGL.eglGetError = dlsym(g_EGL.handle, "eglGetError"))) return (false);
	if (!(g_EGL.eglGetPlatformDisplay = dlsym(g_EGL.handle, "eglGetPlatformDisplay"))) return (false);
	if (!(g_EGL.eglGetProcAddress = dlsym(g_EGL.handle, "eglGetProcAddress"))) return (false);
	if (!(g_EGL.eglGetSyncAttrib = dlsym(g_EGL.handle, "eglGetSyncAttrib"))) return (false);
	if (!(g_EGL.eglInitialize = dlsym(g_EGL.handle, "eglInitialize"))) return (false);
	if (!(g_EGL.eglMakeCurrent = dlsym(g_EGL.handle, "eglMakeCurrent"))) return (false);
	if (!(g_EGL.eglQueryAPI = dlsym(g_EGL.handle, "eglQueryAPI"))) return (false);
	if (!(g_EGL.eglQueryContext = dlsym(g_EGL.handle, "eglQueryContext"))) return (false);
	if (!(g_EGL.eglQueryString = dlsym(g_EGL.handle, "eglQueryString"))) return (false);
	if (!(g_EGL.eglQuerySurface = dlsym(g_EGL.handle, "eglQuerySurface"))) return (false);
	if (!(g_EGL.eglReleaseTexImage = dlsym(g_EGL.handle, "eglReleaseTexImage"))) return (false);
	if (!(g_EGL.eglReleaseThread = dlsym(g_EGL.handle, "eglReleaseThread"))) return (false);
	if (!(g_EGL.eglSurfaceAttrib = dlsym(g_EGL.handle, "eglSurfaceAttrib"))) return (false);
	if (!(g_EGL.eglSwapBuffers = dlsym(g_EGL.handle, "eglSwapBuffers"))) return (false);
	if (!(g_EGL.eglSwapInterval = dlsym(g_EGL.handle, "eglSwapInterval"))) return (false);
	if (!(g_EGL.eglTerminate = dlsym(g_EGL.handle, "eglTerminate"))) return (false);
	if (!(g_EGL.eglWaitClient = dlsym(g_EGL.handle, "eglWaitClient"))) return (false);
	if (!(g_EGL.eglWaitGL = dlsym(g_EGL.handle, "eglWaitGL"))) return (false);
	if (!(g_EGL.eglWaitNative = dlsym(g_EGL.handle, "eglWaitNative"))) return (false);
	if (!(g_EGL.eglWaitSync = dlsym(g_EGL.handle, "eglWaitSync"))) return (false);
	return (true);
}
