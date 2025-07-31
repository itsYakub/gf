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
 *  Private interface declarations
 * */

GFAPIS bool	__gf_sendX11Event(t_window, bool, Atom, Atom);




/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_setWindowResizable(t_window win, bool state) {
	XSizeHints	_hints;
	int64_t		_toto;
	int32_t		_width;
	int32_t		_height;
	
	gf_int_ensureWindow(win);
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

	gf_int_ensureWindow(win);
	_type = state ? win->atoms.wm_net_window_dock : win->atoms.wm_net_window_normal;
	XChangeProperty(
		win->x11.dsp, win->x11.id,
		win->atoms.wm_net_window_type, XA_ATOM,
		32, PropModeReplace,
		(const unsigned char *) &_type, 1
	);
	return (true);
}

GFAPI bool	gf_setWindowFullscreen(t_window win, bool state) {
	gf_int_ensureWindow(win);
	__gf_sendX11Event(win, state, win->atoms.wm_net_state_fullscreen, 0);
	return (true);
}

GFAPI bool	gf_setWindowVSync(t_window win, bool state) {
	int32_t	_interval;

	gf_int_ensureWindow(win);
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





/* SECTION:
 *  Private interface definitions
 * */

GFAPIS bool	__gf_sendX11Event(t_window win, bool state, Atom atom0, Atom atom1) {
	XEvent	_event;
	
	memset(&_event, 0, sizeof(XEvent));
	_event.type = ClientMessage;
	_event.xclient.display = win->x11.dsp;
	_event.xclient.window = win->x11.id;
	_event.xclient.send_event = true;
	_event.xclient.serial = 0;
	_event.xclient.format = 32;
	
	_event.xclient.message_type = win->atoms.wm_net_state;
	_event.xclient.data.l[0] = state ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
	_event.xclient.data.l[1] = atom0;
	_event.xclient.data.l[2] = atom1;
	_event.xclient.data.l[3] = 1;
	
	XSendEvent(win->x11.dsp, win->x11.root_id, 0, SubstructureRedirectMask | SubstructureNotifyMask, &_event);
	XFlush(win->x11.dsp);
	return (true);
}
