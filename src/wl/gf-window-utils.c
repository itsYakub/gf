#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

GFAPI bool	gf_getWindowSize(t_window win, int32_t *wptr, int32_t *hptr) {
	if (wptr) {
		*wptr = win->misc.width;
	}
	if (hptr) {
		*hptr = win->misc.height;
	}
	return (true);
}

GFAPI bool	gf_getMonitorSize(t_window win, int32_t *wptr, int32_t *hptr) {
	(void) win;
	(void) wptr;
	(void) hptr;
	return (true);
}

GFAPI bool	gf_getWindowPosition(t_window win, int32_t *xptr, int32_t *yptr) {
	(void) win;
	(void) xptr;
	(void) yptr;
	return (!gf_loge("WINDOW: Position unavailable\n"));
}

GFAPI bool	gf_getWindowTitle(t_window win, char **tptr) {
	if (!win || !tptr || !*tptr) {
		return (false);
	}
	memcpy(*tptr, win, _GF_WINDOW_TITLE_LEN);
	(*tptr)[_GF_WINDOW_TITLE_LEN] = 0;
	return (true);
}

GFAPI bool	gf_setWindowSize(t_window win, int32_t w, int32_t h) {
	wl_egl_window_resize(win->client.id, w, h, 0, 0);
	win->misc.width = w;
	win->misc.height = h;
	return (true);
}

GFAPI bool	gf_setWindowPosition(t_window win, int32_t x, int32_t y) {
	(void) win;
	(void) x;
	(void) y;
	return (!gf_loge("WINDOW: Position unavailable\n"));
}

GFAPI bool	gf_setWindowTitle(t_window win, const char *t) {
	if (!win->client.xdg.toplevel) {
		return (false);
	}
	xdg_toplevel_set_title(win->client.xdg.toplevel, t);
	memset(win->misc.title, 0, _GF_WINDOW_TITLE_LEN);
	memcpy(win->misc.title, t, strlen(t));
	return (true);
}
