#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

GFAPI bool	gf_getWindowSize(t_window win, int32_t *wptr, int32_t *hptr) {
	XWindowAttributes	_attr;

	memset(&_attr, 0, sizeof(XWindowAttributes));
	if (!XGetWindowAttributes(win->client.dsp, win->client.id, &_attr)) {
		return (false);
	}
	
	/* Updating internal variables
	 * */
	win->data.width = _attr.width;
	win->data.height = _attr.height;

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
		*wptr = DisplayWidth(win->client.dsp, win->client.screen_id);
	}
	if (hptr) {
		*hptr = DisplayHeight(win->client.dsp, win->client.screen_id);
	}
	return (true);
}

GFAPI bool	gf_getWindowPosition(t_window win, int32_t *xptr, int32_t *yptr) {
	XWindowAttributes	_attr;
	Window				_child;
	int32_t				_x, _y;

	memset(&_attr, 0, sizeof(XWindowAttributes));
	if (!XGetWindowAttributes(win->client.dsp, win->client.id, &_attr)) {
		return (false);
	}

	_x = _y = 0;
	XTranslateCoordinates(
		win->client.dsp, win->client.id, win->client.root_id,
		0, 0, &_x, &_y,
		&_child
	);
	
	/* Updating internal variables
	 * */
	win->data.x = _x - _attr.x;
	win->data.y = _y - _attr.y;

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

GFAPI bool	gf_setWindowSize(t_window win, int32_t w, int32_t h) {
	XResizeWindow(win->client.dsp, win->client.id, w, h);
	win->data.width = w;
	win->data.height = h;
	return (true);
}

GFAPI bool	gf_setWindowPosition(t_window win, int32_t x, int32_t y) {
	XMoveWindow(win->client.dsp, win->client.id, x, y);
	win->data.x = x;
	win->data.y = y;
	return (true);
}

GFAPI bool	gf_setWindowTitle(t_window win, const char *t) {
	XStoreName(win->client.dsp, win->client.id, t);
	memset(win->data.title, 0, _GF_WINDOW_TITLE_LEN);
	memcpy(win->data.title, t, strlen(t));
	return (true);
}
