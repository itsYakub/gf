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
 *  Public API implementation
 * */

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
	return (gf_int_updateWindowConfig(win));
}





/* SECTION:
 *  Config Interface
 * */

GFAPII bool	gf_int_updateWindowConfig(t_window win) {

	/* Config: resizable
	 * */
	{
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
	}

	/* Config: borderless
	 * */
	{
		struct s_mwmhints	_hints;
		Atom				_mwm_hints_atom;

		_mwm_hints_atom = XInternAtom(win->client.dsp, "_MOTIF_WM_HINTS", 0);
		_hints = (struct s_mwmhints) { 0 };
		_hints.flags = (1L << 1);
		_hints.decor = (win->config.id & GF_CONFIG_BORDERLESS) ? 0 : 1;
		XChangeProperty(
			win->client.dsp, win->client.id,
			_mwm_hints_atom, _mwm_hints_atom,
			32, PropModeReplace,
			(uint8_t *) &_hints,
			sizeof(_hints) / sizeof(int64_t)
		);
	}

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
