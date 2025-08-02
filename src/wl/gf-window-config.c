#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_setWindowResizable(t_window win, bool state) {
	if (!state) {
		xdg_toplevel_set_min_size(win->xdg.toplevel, win->misc.width, win->misc.height);
		xdg_toplevel_set_max_size(win->xdg.toplevel, win->misc.width, win->misc.height);
	}
	else {
		xdg_toplevel_set_min_size(win->xdg.toplevel, 1, 1);
		xdg_toplevel_set_max_size(win->xdg.toplevel, 0x10000000, 0x10000000);
	}
	return (true);
}

GFAPI bool	gf_setWindowBorderless(t_window win, bool state) {
	(void) win;
	(void) state;
	return (true);
}

GFAPI bool	gf_setWindowFullscreen(t_window win, bool state) {
	state ?
		xdg_toplevel_set_fullscreen(win->xdg.toplevel, 0) :
		xdg_toplevel_unset_fullscreen(win->xdg.toplevel);
	return (true);
}
