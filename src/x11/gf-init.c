#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

static struct s_X11	g_X11 = { 0 };

GFAPII struct s_X11	*gf_int_X11(void) {
	return (&g_X11);
}

GFAPII bool	gf_int_initPlatformX11(void) {
	const char	*_names[] = { "libX11.so", "libX11.so.6", "libX11.so.6.4.0", 0 };
	
	/* Check if the handle isn't already loaded
	 * */
	if (g_X11.handle) {
		return (true);
	}
	/* Open the libX11 shared library
	 * */
	for (size_t i = 0; !g_X11.handle && _names[i]; i++) {
		g_X11.handle = dlopen(_names[i], RTLD_NOW | RTLD_GLOBAL);
	}
	if (!g_X11.handle) {

#if defined (VERBOSE)
		gf_loge("PLATFORM: Failed to load X11 library\n");
#endif

		return (false);
	}
	
	/* Load all the necessary X11 functions
	 * */
	if (!(g_X11.XOpenDisplay = dlsym(g_X11.handle, "XOpenDisplay"))) return (false);
	if (!(g_X11.XCloseDisplay = dlsym(g_X11.handle, "XCloseDisplay"))) return (false);
	if (!(g_X11.XMatchVisualInfo = dlsym(g_X11.handle, "XMatchVisualInfo"))) return (false);
	if (!(g_X11.XCreateColormap = dlsym(g_X11.handle, "XCreateColormap"))) return (false);
	if (!(g_X11.XCreateWindow = dlsym(g_X11.handle, "XCreateWindow"))) return (false);
	if (!(g_X11.XSetWMHints = dlsym(g_X11.handle, "XSetWMHints"))) return (false);
	if (!(g_X11.XSelectInput = dlsym(g_X11.handle, "XSelectInput"))) return (false);
	if (!(g_X11.XMapWindow = dlsym(g_X11.handle, "XMapWindow"))) return (false);
	if (!(g_X11.XDestroyWindow = dlsym(g_X11.handle, "XDestroyWindow"))) return (false);
	if (!(g_X11.XUnmapWindow = dlsym(g_X11.handle, "XUnmapWindow"))) return (false);
	if (!(g_X11.XInternAtom = dlsym(g_X11.handle, "XInternAtom"))) return (false);
	if (!(g_X11.XSetWMProtocols = dlsym(g_X11.handle, "XSetWMProtocols"))) return (false);
	if (!(g_X11.XPending = dlsym(g_X11.handle, "XPending"))) return (false);
	if (!(g_X11.XNextEvent = dlsym(g_X11.handle, "XNextEvent"))) return (false);
	if (!(g_X11.XSendEvent = dlsym(g_X11.handle, "XSendEvent"))) return (false);
	if (!(g_X11.XFlush = dlsym(g_X11.handle, "XFlush"))) return (false);
	if (!(g_X11.XFree = dlsym(g_X11.handle, "XFree"))) return (false);
	if (!(g_X11.XGetWindowProperty = dlsym(g_X11.handle, "XGetWindowProperty"))) return (false);
	if (!(g_X11.XGetWindowAttributes = dlsym(g_X11.handle, "XGetWindowAttributes"))) return (false);
	if (!(g_X11.XTranslateCoordinates = dlsym(g_X11.handle, "XTranslateCoordinates"))) return (false);
	if (!(g_X11.XResizeWindow = dlsym(g_X11.handle, "XResizeWindow"))) return (false);
	if (!(g_X11.XMoveWindow = dlsym(g_X11.handle, "XMoveWindow"))) return (false);
	if (!(g_X11.XStoreName = dlsym(g_X11.handle, "XStoreName"))) return (false);
	if (!(g_X11.XGetWMNormalHints = dlsym(g_X11.handle, "XGetWMNormalHints"))) return (false);
	if (!(g_X11.XSetWMNormalHints = dlsym(g_X11.handle, "XSetWMNormalHints"))) return (false);
	if (!(g_X11.XChangeProperty = dlsym(g_X11.handle, "XChangeProperty"))) return (false);
	if (!(g_X11.XkbKeycodeToKeysym = dlsym(g_X11.handle, "XkbKeycodeToKeysym"))) return (false);
	return (true);
}

GFAPII bool	gf_int_closePlatformX11(void) {
	if (dlclose(g_X11.handle)) {
		return (false);
	}
	g_X11.handle = 0;
	return (true);
}
