#include "./../gf.h"
#include "./gf-int.h"

#include <string.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GL/glx.h>
#include <GL/glxext.h>

#include <GL/gl.h>

/* SECTION:
 *  Static globals
 * */

static PFNGLXSWAPINTERVALEXTPROC	glXSwapIntervalEXT;

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_setWindowResizable(t_window win, bool state) {
	XSizeHints	_hints;
	int64_t		_toto;
	int32_t		_width;
	int32_t		_height;
	
	_width = _height = 0;
	gf_getWindowSize(win, &_width, &_height);
	XGetWMNormalHints(win->x11.dsp, win->x11.id, &_hints, &_toto);	
	_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	_hints.width = _width;
	_hints.height = _height;
	_hints.min_width = 1;
	_hints.max_width = state ? _width : 0x10000000;
	_hints.min_height = 1;
	_hints.max_height = state ? _height : 0x10000000;
	XSetWMNormalHints(win->x11.dsp, win->x11.id, &_hints);
	return (true);
}

GFAPI bool	gf_setWindowBorderless(t_window win, bool state) {
	Atom	_type;

	_type = state ? win->atoms.wm_net_window_dock : win->atoms.wm_net_window_normal;
	XChangeProperty(
		win->x11.dsp, win->x11.id,
		win->atoms.wm_net_window_type, XA_ATOM,
		32, PropModeReplace,
		(const unsigned char *) &_type, 1
	);
	return (true);
}

GFAPI bool	gf_setWindowTopMost(t_window win, bool state) {
	XClientMessageEvent	_event;
	
	memset(&_event, 0, sizeof(XClientMessageEvent));
	_event.type = ClientMessage;
	_event.display = win->x11.dsp;
	_event.window = win->x11.id;
	_event.send_event = true;
	_event.serial = 0;
	_event.format = 32;
	
	_event.message_type = win->atoms.wm_net_state;
	_event.data.l[0] = state ? 1 : 0;
	_event.data.l[1] = win->atoms.wm_net_state_above;
	_event.data.l[3] = 1;
	
	XSendEvent(win->x11.dsp, DefaultRootWindow(win->x11.dsp), 0, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *) &_event);
	XFlush(win->x11.dsp);
	return (true);
}

GFAPI bool	gf_setWindowVSync(t_window win, bool state) {
	int32_t	_interval;

	_interval = state ? 1 : 0;

#if defined (USE_GLX)
	glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((GLubyte *) "glXSwapIntervalEXT");
	if (!glXSwapIntervalEXT) {
		return (false);
	}
	glXSwapIntervalEXT(win->x11.dsp, win->x11.id, _interval);
#elif defined (USE_EGL)
	(void) glXSwapIntervalEXT;
	eglSwapInterval(win->egl.dsp, _interval);
#else
	return (false);
#endif

	return (true);
}
