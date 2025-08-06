#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyWindow(t_window win) {
	if (win->xkb.state) xkb_state_unref(win->xkb.state);
	if (win->xkb.keymap) xkb_keymap_unref(win->xkb.keymap);
	if (win->xkb.ctx) xkb_context_unref(win->xkb.ctx);
	
	if (win->xdg.toplevel) xdg_toplevel_destroy(win->xdg.toplevel);
	if (win->xdg.surf) xdg_surface_destroy(win->xdg.surf);
	if (win->xdg.base) xdg_wm_base_destroy(win->xdg.base);

	if (win->wl.pointer) wl_pointer_destroy(win->wl.pointer);
	if (win->wl.keyboard) wl_keyboard_destroy(win->wl.keyboard);
	if (win->wl.seat) wl_seat_destroy(win->wl.seat);

	if (win->wl.surf) wl_surface_destroy(win->wl.surf);
	if (win->wl.comp) wl_compositor_destroy(win->wl.comp);
	if (win->wl.reg) wl_registry_destroy(win->wl.reg);

	if (win->wl.id) wl_egl_window_destroy(win->wl.id);

	if (win->wl.dsp) wl_display_disconnect(win->wl.dsp);

	free(win);
	return (true);
}
