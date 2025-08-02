#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyWindow(t_window win) {
	xdg_toplevel_destroy(win->xdg.toplevel);
	xdg_surface_destroy(win->xdg.surf);
	xdg_wm_base_destroy(win->xdg.base);

#if defined (VERBOSE)
	gf_logi("XDG: Terminated successfully\n");
#endif

	wl_pointer_destroy(win->wl.pointer);
	wl_keyboard_destroy(win->wl.keyboard);
	wl_seat_destroy(win->wl.seat);

	wl_surface_destroy(win->wl.surf);
	wl_compositor_destroy(win->wl.comp);
	wl_registry_destroy(win->wl.reg);

	wl_egl_window_destroy(win->wl.id);

#if defined (VERBOSE)
	gf_logi("WINDOW: Destroyed successfully\n");
#endif

	wl_display_disconnect(win->wl.dsp);

#if defined (VERBOSE)
	gf_logi("DISPLAY: Destroyed successfully\n");
#endif

	free(win);
	return (true);
}
