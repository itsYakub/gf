#include "./../gf.h"
#include "./gf-int.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GL/glx.h>
#include <GL/glxext.h>

#include <GL/gl.h>

/* SECTION:
 *  Static globals
 * */

static PFNGLXSWAPINTERVALEXTPROC			glXSwapIntervalEXT;

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_setWindowResizable(t_window win, bool state) {
	XSizeHints	_hints;
	int64_t		_toto;
	int32_t		_width;
	int32_t		_height;
	
	XGetWMNormalHints(win->x11.dsp, win->x11.id, &_hints, &_toto);
	_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	
	_width = _height = 0;
	gf_getWindowSize(win, &_width, &_height);
	_hints.width = _width;
	_hints.height = _height;
	
	if (state) {
		gf_getMonitorSize(win, &_width, &_height);
		_hints.min_width = 1;
		_hints.max_width = _width;
		_hints.min_height = 1;
		_hints.max_height = _height;
	}
	else {	
		_hints.min_width = _width;
		_hints.max_width = _width;
		_hints.min_height = _height;
		_hints.max_height = _height;
	}

	XSetWMNormalHints(win->x11.dsp, win->x11.id, &_hints);
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
	eglSwapInterval(win->egl.dsp, win->egl.surf, _interval);
#else
	return (false);
#endif

	return (true);
}
