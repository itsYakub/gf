#include "gf-int-x11.h"
#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Static globals
 * */

struct s_mwmhints {
	uint64_t	flags;
	uint64_t	funcs;
	uint64_t	decor;
	int64_t		input;
	uint64_t	stat;
};



/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_sendEvent(t_window, Atom, Atom, Atom, int64_t state);
GFAPIS bool	__gf_configResizable(t_window);
GFAPIS bool	__gf_configBorderless(t_window);




/* SECTION:
 *  Public API implementation
 * */

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

GFAPI bool	gf_setWindowConfig(t_window win, int32_t conf) {
	/* Updating the resizable state
	 * */
	if (conf & GF_CONFIG_RESIZABLE) {
		if (win->config.id & GF_CONFIG_RESIZABLE) win->config.id &= ~GF_CONFIG_RESIZABLE;
		else win->config.id |= GF_CONFIG_RESIZABLE;
	}

	/* Updating the borderless state
	 * */
	if (conf & GF_CONFIG_BORDERLESS) {
		if (win->config.id & GF_CONFIG_BORDERLESS) win->config.id &= ~GF_CONFIG_BORDERLESS;
		else win->config.id |= GF_CONFIG_BORDERLESS;
	}

	/* Updating the topmost state
	 * */
	if (conf & GF_CONFIG_TOPMOST) {
		if (win->config.id & GF_CONFIG_TOPMOST) win->config.id &= ~GF_CONFIG_TOPMOST;
		else win->config.id |= GF_CONFIG_TOPMOST;
	}
	
	/* Updating the fullscreen state
	 * */
	if (conf & GF_CONFIG_FULLSCREEN) {
		if (win->config.id & GF_CONFIG_FULLSCREEN) win->config.id &= ~GF_CONFIG_FULLSCREEN;
		else win->config.id |= GF_CONFIG_FULLSCREEN;
	}
	
	/* Updating the minimized state
	 * */
	if (conf & GF_CONFIG_MINIMIZED) {
		if (win->config.id & GF_CONFIG_MINIMIZED) win->config.id &= ~GF_CONFIG_MINIMIZED;
		else win->config.id |= GF_CONFIG_MINIMIZED;
	}
	
	/* Updating the maximized state
	 * */
	if (conf & GF_CONFIG_MAXIMIZED) {
		if (win->config.id & GF_CONFIG_MAXIMIZED) win->config.id &= ~GF_CONFIG_MAXIMIZED;
		else win->config.id |= GF_CONFIG_MAXIMIZED;
	}
	return (gf_int_updateWindowConfig(win));
}





/* SECTION:
 *  Config Interface
 * */

GFAPII bool	gf_int_updateWindowConfig(t_window win) {

	/* Config: resizable
	 * */
	__gf_configResizable(win);

	/* Config: borderless
	 * */
	__gf_configBorderless(win);
	
	/* Config: topmost
	 * TODO(yakub):
	 *  This doesn't work on i3wm. Test on other DE
	 * */
	__gf_sendEvent(win,
		win->client.atoms._NET_WM_STATE, win->client.atoms._NET_WM_STATE_ABOVE, 0,
		(win->config.id & GF_CONFIG_TOPMOST) ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE
	);
	
	/* Config: fullscreen
	 * */
	__gf_sendEvent(win,
		win->client.atoms._NET_WM_STATE, win->client.atoms._NET_WM_STATE_FULLSCREEN, 0,
		(win->config.id & GF_CONFIG_FULLSCREEN) ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE
	);
	
	/* Config: minimized
	 * TODO(yakub):
	 *  This doesn't work on i3wm. Test on other DE
	 * */
	__gf_sendEvent(win,
		win->client.atoms.WM_CHANGE_STATE, win->client.atoms._NET_WM_STATE_HIDDEN, 0,
		(win->config.id & GF_CONFIG_MINIMIZED) ? IconicState : NormalState
	);

	/* Config: maximized
	 * */
	__gf_sendEvent(win,
		win->client.atoms._NET_WM_STATE, win->client.atoms._NET_WM_STATE_MAXIMIZED_HORZ, win->client.atoms._NET_WM_STATE_MAXIMIZED_VERT,
		(win->config.id & GF_CONFIG_MAXIMIZED) ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE
	);

	/* Last step: force-update window manager
	 * */
	{
		XMapEvent	_event;

		_event = (XMapEvent) { 0 };
		_event.display = win->client.dsp;
		_event.window = win->client.id;
		_event.type = MapNotify;
		XSendEvent(win->client.dsp, win->client.id, 0, 0, (XEvent *) &_event);
		gf_flushEvents(win);
	}
	return (true);
}





/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_sendEvent(t_window win, Atom atom0, Atom atom1, Atom atom2, int64_t state) {
	XClientMessageEvent	_event;

	_event = (XClientMessageEvent) { 0 };
	_event.display = win->client.dsp;
	_event.window = win->client.id;
	_event.type = ClientMessage;
	_event.format = 32;
	_event.message_type = atom0;

	_event.data.l[0] = state;
	_event.data.l[1] = atom1;
	_event.data.l[2] = atom2;
	_event.data.l[3] = 0;
	_event.data.l[4] = 0;

	XSendEvent(win->client.dsp, win->client.root_id, 0, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *) &_event);
	gf_flushEvents(win);
	return (true);
}

GFAPIS bool	__gf_configResizable(t_window win) {
	XSizeHints	_hints;
	long		_supp;

	XGetWMNormalHints(win->client.dsp, win->client.id, &_hints, &_supp);
	_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	if (win->config.id & GF_CONFIG_RESIZABLE) {
		_hints.flags |= PMinSize;
		_hints.min_width = _hints.min_height = 1;
		_hints.flags |= PMaxSize;
		_hints.max_width = _hints.max_height = 0x10000000;
	}
	else {
		_hints.flags |= PMinSize;
		_hints.min_width = _hints.max_width = win->data.width;
		_hints.flags |= PMaxSize;
		_hints.min_height = _hints.max_height = win->data.height;
	}
	XSetWMNormalHints(win->client.dsp, win->client.id, &_hints);
	return (true);
}

GFAPIS bool	__gf_configBorderless(t_window win) {
	struct s_mwmhints	_hints;

	_hints = (struct s_mwmhints) { 0 };
	_hints.flags = (1L << 1);
	_hints.decor = (win->config.id & GF_CONFIG_BORDERLESS) ? 0 : 1;
	XChangeProperty(
		win->client.dsp, win->client.id,
		win->client.atoms._MOTIF_WM_HINTS, XA_ATOM,
		32, PropModeReplace,
		(uint8_t *) &_hints,
		sizeof(_hints) / sizeof(int64_t)
	);
	return (true);
}
