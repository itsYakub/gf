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

	(*win)->wl.id = wl_egl_window_create((*win)->wl.surf, w, h);
	if (!(*win)->wl.id) {

#if defined (VERBOSE)
		gf_loge("WINDOW: Failed to create\n");
#endif

		return (false);
	}
	
	if (t) {
		gf_setWindowTitle(*win, t);
	}

	wl_surface_commit((*win)->wl.surf);

	/* Another roundtrip to create all the remaining structures, especially wl_seat
	 * */
	wl_display_roundtrip((*win)->wl.dsp);

	return (true);
}





/* SECTION:
 *  Private interface definitions
 * */

GFAPIS bool	__gf_connectDisplay(t_window win) {
	win->wl.dsp = wl_display_connect(0);
	if (!win->wl.dsp) {

#if defined (VERBOSE)
		gf_loge("DISPLAY: Failed to connect\n");
#endif

		return (false);
	}

	win->wl.reg = wl_display_get_registry(win->wl.dsp);
	if (!win->wl.reg) {

#if defined (VERBOSE)
		gf_loge("REGISTRY: Failed to get\n");
#endif

		return (false);
	}

	wl_registry_add_listener(win->wl.reg, &win->listener.registry, win);
	wl_display_roundtrip(win->wl.dsp);
	return (win->wl.comp && win->xdg.base);
}

GFAPIS bool	__gf_createSurface(t_window win) {
	win->wl.surf = wl_compositor_create_surface(win->wl.comp);
	if (!win->wl.surf) {

#if defined (VERBOSE)
		gf_loge("COMPOSITOR: Failed to create a surface\n");
#endif

		return (false);
	}
	
	return (true);
}

GFAPIS bool	__gf_createXDG(t_window win) {
	win->xdg.surf = xdg_wm_base_get_xdg_surface(win->xdg.base, win->wl.surf);
	if (!win->xdg.surf) {

#if defined (VERBOSE)
		gf_loge("XDG: Failed to create a surface\n");
#endif

		return (false);
	}
	xdg_surface_add_listener(win->xdg.surf, &win->listener.surface, win);

	win->xdg.toplevel = xdg_surface_get_toplevel(win->xdg.surf);
	if (!win->xdg.toplevel) {

#if defined (VERBOSE)
		gf_loge("XDG: Failed to create a toplevel\n");
#endif

		return (false);
	}
	xdg_toplevel_add_listener(win->xdg.toplevel, &win->listener.toplevel, win);
	return (true);
}
