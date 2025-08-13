#include "gf-int-x11.h"
#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

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
		XSetWindowAttributes	_attr;
			
		if (win->config.id & GF_CONFIG_BORDERLESS) {
			_attr = (XSetWindowAttributes) { 0 };
			_attr.override_redirect = true;
			XChangeWindowAttributes(win->client.dsp, win->client.id, CWOverrideRedirect, &_attr);
		}
		else {
			_attr = (XSetWindowAttributes) { 0 };
			_attr.override_redirect = true;
		}
		XChangeWindowAttributes(win->client.dsp, win->client.id, CWOverrideRedirect, &_attr);
	}
	return (true);
}
