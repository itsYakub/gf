#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyWindow(t_window win) {
	if (win->client.xkb.state) xkb_state_unref(win->client.xkb.state);
	if (win->client.xkb.keymap) xkb_keymap_unref(win->client.xkb.keymap);
	if (win->client.xkb.ctx) xkb_context_unref(win->client.xkb.ctx);
	
	if (win->client.xdg.toplevel) xdg_toplevel_destroy(win->client.xdg.toplevel);
	if (win->client.xdg.surf) xdg_surface_destroy(win->client.xdg.surf);
	if (win->client.xdg.base) xdg_wm_base_destroy(win->client.xdg.base);

	if (win->client.pointer) wl_pointer_destroy(win->client.pointer);
	if (win->client.keyboard) wl_keyboard_destroy(win->client.keyboard);
	if (win->client.seat) wl_seat_destroy(win->client.seat);

	if (win->client.surf) wl_surface_destroy(win->client.surf);
	if (win->client.comp) wl_compositor_destroy(win->client.comp);
	if (win->client.reg) wl_registry_destroy(win->client.reg);

	if (win->client.id) wl_egl_window_destroy(win->client.id);

	if (win->client.dsp) wl_display_disconnect(win->client.dsp);

	free(win);
	return (true);
}
