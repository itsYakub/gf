#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

struct s_X11	g_X11 = { 0 };

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

	/* SECTION: Xlib.h
	 * */	
	if (!(g_X11.XOpenDisplay = dlsym(g_X11.handle, "XOpenDisplay"))) return (false);
	if (!(g_X11.XCloseDisplay = dlsym(g_X11.handle, "XCloseDisplay"))) return (false);
	if (!(g_X11.XCreateColormap = dlsym(g_X11.handle, "XCreateColormap"))) return (false);
	if (!(g_X11.XCreateWindow = dlsym(g_X11.handle, "XCreateWindow"))) return (false);
	if (!(g_X11.XSelectInput = dlsym(g_X11.handle, "XSelectInput"))) return (false);
	if (!(g_X11.XMapWindow = dlsym(g_X11.handle, "XMapWindow"))) return (false);
	if (!(g_X11.XDestroyWindow = dlsym(g_X11.handle, "XDestroyWindow"))) return (false);
	if (!(g_X11.XUnmapWindow = dlsym(g_X11.handle, "XUnmapWindow"))) return (false);
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
	if (!(g_X11.XChangeProperty = dlsym(g_X11.handle, "XChangeProperty"))) return (false);

	/* SECTION: Xutil.h
	 * */	
	if (!(g_X11.XAllocColor = dlsym(g_X11.handle, "XAllocColor"))) return (false);
	if (!(g_X11.XAllocWMHints = dlsym(g_X11.handle, "XAllocWMHints"))) return (false);
	if (!(g_X11.XChangeWindowAttributes = dlsym(g_X11.handle, "XChangeWindowAttributes"))) return (false);
	if (!(g_X11.XGetGeometry = dlsym(g_X11.handle, "XGetGeometry"))) return (false);
	if (!(g_X11.XGetWMHints = dlsym(g_X11.handle, "XGetWMHints"))) return (false);
	if (!(g_X11.XGetWMNormalHints = dlsym(g_X11.handle, "XGetWMNormalHints"))) return (false);
	if (!(g_X11.XGetWMSizeHints = dlsym(g_X11.handle, "XGetWMSizeHints"))) return (false);
	if (!(g_X11.XGetIconSizes = dlsym(g_X11.handle, "XGetIconSizes"))) return (false);
	if (!(g_X11.XGetNormalHints = dlsym(g_X11.handle, "XGetNormalHints"))) return (false);
	if (!(g_X11.XGetSizeHints = dlsym(g_X11.handle, "XGetSizeHints"))) return (false);
	if (!(g_X11.XGetWMColormapWindows = dlsym(g_X11.handle, "XGetWMColormapWindows"))) return (false);
	if (!(g_X11.XGetWMProtocols = dlsym(g_X11.handle, "XGetWMProtocols"))) return (false);
	if (!(g_X11.XGetZoomHints = dlsym(g_X11.handle, "XGetZoomHints"))) return (false);
	if (!(g_X11.XIconifyWindow = dlsym(g_X11.handle, "XIconifyWindow"))) return (false);
	if (!(g_X11.XLookupColor = dlsym(g_X11.handle, "XLookupColor"))) return (false);
	if (!(g_X11.XSetWMColormapWindows = dlsym(g_X11.handle, "XSetWMColormapWindows"))) return (false);
	if (!(g_X11.XSetWMHints = dlsym(g_X11.handle, "XSetWMHints"))) return (false);
	if (!(g_X11.XSetWMIconName = dlsym(g_X11.handle, "XSetWMIconName"))) return (false);
	if (!(g_X11.XSetWMName = dlsym(g_X11.handle, "XSetWMName"))) return (false);
	if (!(g_X11.XSetWMNormalHints = dlsym(g_X11.handle, "XSetWMNormalHints"))) return (false);
	if (!(g_X11.XSetWMProtocols = dlsym(g_X11.handle, "XSetWMProtocols"))) return (false);
	if (!(g_X11.XSetWMSizeHints = dlsym(g_X11.handle, "XSetWMSizeHints"))) return (false);
	if (!(g_X11.XWithdrawWindow = dlsym(g_X11.handle, "XWithdrawWindow"))) return (false);
	if (!(g_X11.XMatchVisualInfo = dlsym(g_X11.handle, "XMatchVisualInfo"))) return (false);

	/* SECTION: Xatom.h
	 * */
	if (!(g_X11.XInternAtom = dlsym(g_X11.handle, "XInternAtom"))) return (false);
	if (!(g_X11.XInternAtoms = dlsym(g_X11.handle, "XInternAtoms"))) return (false);

	/* SECTION: XKBlib.h
	 * */
    if (!(g_X11.XkbQueryExtension = dlsym(g_X11.handle, "XkbQueryExtension"))) return (false);
    if (!(g_X11.XkbKeycodeToKeysym = dlsym(g_X11.handle, "XkbKeycodeToKeysym"))) return (false);
    if (!(g_X11.XkbLibraryVersion = dlsym(g_X11.handle, "XkbLibraryVersion"))) return (false);
    if (!(g_X11.XkbGetKeyboard = dlsym(g_X11.handle, "XkbGetKeyboard"))) return (false);
    if (!(g_X11.XkbGetMap = dlsym(g_X11.handle, "XkbGetMap"))) return (false);

	return (true);
}
