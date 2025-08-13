#if !defined USE_WL && !defined USE_EGL
# define USE_WL
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"
#include "./gf-int-wl.h"

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_connectDisplay(t_window);
GFAPIS bool	__gf_createSurface(t_window);
GFAPIS bool	__gf_createXDG(t_window);





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createWindow(t_window *win, const size_t w, const size_t h, const char *t) {
	*win = (t_window) calloc(1, sizeof(struct s_window));

	assert(gf_int_initListeners(*win));
	assert(__gf_connectDisplay(*win));
	assert(__gf_createSurface(*win));
	assert(__gf_createXDG(*win));

	(*win)->client.id = wl_egl_window_create((*win)->client.surf, w, h);
	if (!(*win)->client.id) {

#if defined (VERBOSE)
		gf_loge("WINDOW: Failed to create\n");
#endif

		return (false);
	}
	
	if (t) {
		gf_setWindowTitle(*win, t);
	}

	wl_surface_commit((*win)->client.surf);

	/* Another roundtrip to create all the remaining structures, especially wl_seat
	 * */
	wl_display_roundtrip((*win)->client.dsp);

	return (true);
}





/* SECTION:
 *  Private interface definitions
 * */

GFAPIS bool	__gf_connectDisplay(t_window win) {
	win->client.dsp = wl_display_connect(0);
	if (!win->client.dsp) {

#if defined (VERBOSE)
		gf_loge("DISPLAY: Failed to connect\n");
#endif

		return (false);
	}

	win->client.reg = wl_display_get_registry(win->client.dsp);
	if (!win->client.reg) {

#if defined (VERBOSE)
		gf_loge("REGISTRY: Failed to get\n");
#endif

		return (false);
	}

	wl_registry_add_listener(win->client.reg, &win->client.listener.registry, win);
	wl_display_roundtrip(win->client.dsp);
	return (win->client.comp && win->client.xdg.base);
}

GFAPIS bool	__gf_createSurface(t_window win) {
	win->client.surf = wl_compositor_create_surface(win->client.comp);
	if (!win->client.surf) {

#if defined (VERBOSE)
		gf_loge("COMPOSITOR: Failed to create a surface\n");
#endif

		return (false);
	}
	
	return (true);
}

GFAPIS bool	__gf_createXDG(t_window win) {
	win->client.xdg.surf = xdg_wm_base_get_xdg_surface(win->client.xdg.base, win->client.surf);
	if (!win->client.xdg.surf) {

#if defined (VERBOSE)
		gf_loge("XDG: Failed to create a surface\n");
#endif

		return (false);
	}
	xdg_surface_add_listener(win->client.xdg.surf, &win->client.listener.surface, win);

	win->client.xdg.toplevel = xdg_surface_get_toplevel(win->client.xdg.surf);
	if (!win->client.xdg.toplevel) {

#if defined (VERBOSE)
		gf_loge("XDG: Failed to create a toplevel\n");
#endif

		return (false);
	}
	xdg_toplevel_add_listener(win->client.xdg.toplevel, &win->client.listener.toplevel, win);
	return (true);
}
