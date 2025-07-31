#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

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
		*wptr = DisplayWidth(win->x11.dsp, win->x11.screen_id);
	}
	if (hptr) {
		*hptr = DisplayHeight(win->x11.dsp, win->x11.screen_id);
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
		win->x11.dsp, win->x11.id, win->x11.root_id,
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
