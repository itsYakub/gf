#include "./../gf.h"
#include "./gf-int.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

GFAPI bool	gf_getWindowSize(t_window win, int32_t *wptr, int32_t *hptr) {
	XWindowAttributes	_attr;

	memset(&_attr, 0, sizeof(XWindowAttributes));
	if (!XGetWindowAttributes(win->x11.dsp, win->x11.id, &_attr)) {
		return (false);
	}
	
	/* Updating internal variables
	 * */
	win->misc.width = _attr.width;
	win->misc.height = _attr.height;

	/* Setting the value of the 'wptr' and 'hptr' (memory safe)
	 * */
	if (wptr) {
		*wptr = _attr.width;
	}
	if (hptr) {
		*hptr = _attr.height;
	}
	return (true);
}

GFAPI bool	gf_getMonitorSize(t_window win, int32_t *wptr, int32_t *hptr) {
	/* Setting the value of the 'wptr' and 'hptr' (memory safe)
	 * */
	if (wptr) {
		*wptr = DisplayWidth(win->x11.dsp, DefaultScreen(win->x11.dsp));
	}
	if (hptr) {
		*hptr = DisplayHeight(win->x11.dsp, DefaultScreen(win->x11.dsp));
	}
	return (true);
}

GFAPI bool	gf_getWindowPosition(t_window win, int32_t *xptr, int32_t *yptr) {
	XWindowAttributes	_attr;
	Window				_child;
	int32_t				_x, _y;

	memset(&_attr, 0, sizeof(XWindowAttributes));
	if (!XGetWindowAttributes(win->x11.dsp, win->x11.id, &_attr)) {
		return (false);
	}

	_x = _y = 0;
	XTranslateCoordinates(
		win->x11.dsp, win->x11.id, DefaultRootWindow(win->x11.dsp),
		0, 0, &_x, &_y,
		&_child
	);
	
	/* Updating internal variables
	 * */
	win->misc.x = _x - _attr.x;
	win->misc.y = _y - _attr.y;

	/* Setting the value of the 'wptr' and 'hptr' (memory safe)
	 * */
	if (xptr) {
		*xptr = _attr.x;
	}
	if (yptr) {
		*yptr = _attr.y;
	}
	return (true);
}

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
