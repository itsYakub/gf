#if !defined (USE_GLX)
# define USE_GLX
#endif
#include "./../gf-int.h"
#include "./../gf.h"

struct s_GLX	g_GLX = { 0 };

GFAPII bool	gf_int_initPlatformGLX(void) {
	const char	*_names[] = { "libGL.so", "libGL.so.1", 0 };
	
	/* Check if the handle isn't already loaded
	* */
	if (g_GLX.handle) {
		return (true);
	}
	/* Open the libGL shared library
	* */
	for (size_t i = 0; !g_GLX.handle && _names[i]; i++) {
		g_GLX.handle = dlopen(_names[i], RTLD_NOW | RTLD_GLOBAL);
	}
	if (!g_GLX.handle) {

#if defined (VERBOSE)
		gf_loge("PLATFORM: Failed to load GLX library\n");
#endif

		return (false);
	}
	
	/* Load all the necessary GLX functions
	* */
	if (!(g_GLX.glXQueryExtension = dlsym(g_GLX.handle, "glXQueryExtension"))) return (false);
	if (!(g_GLX.glXQueryVersion = dlsym(g_GLX.handle, "glXQueryVersion"))) return (false);
	if (!(g_GLX.glXQueryExtensionsString = dlsym(g_GLX.handle, "glXQueryExtensionsString"))) return (false);
	if (!(g_GLX.glXGetClientString = dlsym(g_GLX.handle, "glXGetClientString"))) return (false);
	if (!(g_GLX.glXQueryServerString = dlsym(g_GLX.handle, "glXQueryServerString"))) return (false);
	if (!(g_GLX.glXGetFBConfigs = dlsym(g_GLX.handle, "glXGetFBConfigs"))) return (false);
	if (!(g_GLX.glXChooseFBConfig = dlsym(g_GLX.handle, "glXChooseFBConfig"))) return (false);
	if (!(g_GLX.glXGetFBConfigAttrib = dlsym(g_GLX.handle, "glXGetFBConfigAttrib"))) return (false);
	if (!(g_GLX.glXGetVisualFromFBConfig = dlsym(g_GLX.handle, "glXGetVisualFromFBConfig"))) return (false);
	if (!(g_GLX.glXCreateWindow = dlsym(g_GLX.handle, "glXCreateWindow"))) return (false);
	if (!(g_GLX.glXDestroyWindow = dlsym(g_GLX.handle, "glXDestroyWindow"))) return (false);
	if (!(g_GLX.glXCreatePixmap = dlsym(g_GLX.handle, "glXCreatePixmap"))) return (false);
	if (!(g_GLX.glXDestroyPixmap = dlsym(g_GLX.handle, "glXDestroyPixmap"))) return (false);
	if (!(g_GLX.glXCreatePbuffer = dlsym(g_GLX.handle, "glXCreatePbuffer"))) return (false);
	if (!(g_GLX.glXDestroyPbuffer = dlsym(g_GLX.handle, "glXDestroyPbuffer"))) return (false);
	if (!(g_GLX.glXQueryDrawable = dlsym(g_GLX.handle, "glXQueryDrawable"))) return (false);
	if (!(g_GLX.glXCreateNewContext = dlsym(g_GLX.handle, "glXCreateNewContext"))) return (false);
	if (!(g_GLX.glXIsDirect = dlsym(g_GLX.handle, "glXIsDirect"))) return (false);
	if (!(g_GLX.glXDestroyContext = dlsym(g_GLX.handle, "glXDestroyContext"))) return (false);
	if (!(g_GLX.glXMakeContextCurrent = dlsym(g_GLX.handle, "glXMakeContextCurrent"))) return (false);
	if (!(g_GLX.glXCopyContext = dlsym(g_GLX.handle, "glXCopyContext"))) return (false);
	if (!(g_GLX.glXGetCurrentContext = dlsym(g_GLX.handle, "glXGetCurrentContext"))) return (false);
	if (!(g_GLX.glXGetCurrentDrawable = dlsym(g_GLX.handle, "glXGetCurrentDrawable"))) return (false);
	if (!(g_GLX.glXGetCurrentReadDrawable = dlsym(g_GLX.handle, "glXGetCurrentReadDrawable"))) return (false);
	if (!(g_GLX.glXGetCurrentDisplay = dlsym(g_GLX.handle, "glXGetCurrentDisplay"))) return (false);
	if (!(g_GLX.glXQueryContext = dlsym(g_GLX.handle, "glXQueryContext"))) return (false);
	if (!(g_GLX.glXSelectEvent = dlsym(g_GLX.handle, "glXSelectEvent"))) return (false);
	if (!(g_GLX.glXGetSelectedEvent = dlsym(g_GLX.handle, "glXGetSelectedEvent"))) return (false);
	if (!(g_GLX.glXWaitGL = dlsym(g_GLX.handle, "glXWaitGL"))) return (false);
	if (!(g_GLX.glXWaitX = dlsym(g_GLX.handle, "glXWaitX"))) return (false);
	if (!(g_GLX.glXSwapBuffers = dlsym(g_GLX.handle, "glXSwapBuffers"))) return (false);
	if (!(g_GLX.glXUseXFont = dlsym(g_GLX.handle, "glXUseXFont"))) return (false);
	if (!(g_GLX.glXGetProcAddress = dlsym(g_GLX.handle, "glXGetProcAddress"))) return (false);
	if (!(g_GLX.glXGetConfig = dlsym(g_GLX.handle, "glXGetConfig"))) return (false);
	if (!(g_GLX.glXChooseVisual = dlsym(g_GLX.handle, "glXChooseVisual"))) return (false);
	if (!(g_GLX.glXCreateGLXPixmap = dlsym(g_GLX.handle, "glXCreateGLXPixmap"))) return (false);
	if (!(g_GLX.glXDestroyGLXPixmap = dlsym(g_GLX.handle, "glXDestroyGLXPixmap"))) return (false);
	if (!(g_GLX.glXCreateContext = dlsym(g_GLX.handle, "glXCreateContext"))) return (false);
	if (!(g_GLX.glXMakeCurrent = dlsym(g_GLX.handle, "glXMakeCurrent"))) return (false);
	if (!(g_GLX.glXCreateContextAttribsARB = dlsym(g_GLX.handle, "glXCreateContextAttribsARB"))) return (false);
	return (true);
}
