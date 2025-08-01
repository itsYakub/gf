#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_connectDisplay(t_window);



/* * Wayland configuration functions
 * * */

GFAPIS void __wl_registry_global(void *, struct wl_registry *, uint32_t, const char *, uint32_t);
GFAPIS void	__wl_registry_global_remove(void *, struct wl_registry *, uint32_t);

GFAPIS void	__xdg_wm_base_ping(void *, struct xdg_wm_base *, uint32_t);

GFAPIS void	__xdg_surface_configure(void *, struct xdg_surface *, uint32_t);

GFAPIS void	__xdg_toplevel_configure(void *, struct xdg_toplevel *, int32_t, int32_t, struct wl_array *);
GFAPIS void	__xdg_toplevel_close(void *, struct xdg_toplevel *);
GFAPIS void	__xdg_toplevel_configure_bounds(void *, struct xdg_toplevel *, int32_t, int32_t);
GFAPIS void	__xdg_toplevel_wm_capabilities(void *, struct xdg_toplevel *, struct wl_array *);





/* SECTION:
 *  Static globals
 * */

static struct wl_registry_listener	g_registry_listener = {
	.global = __wl_registry_global,
	.global_remove = __wl_registry_global_remove
};

static struct xdg_wm_base_listener	g_wm_base_listener = {
	.ping = __xdg_wm_base_ping
};

static struct xdg_surface_listener	g_surface_xdg_listener = {
	.configure = __xdg_surface_configure
};

static struct xdg_toplevel_listener	g_toplevel_listener = {
	.close = __xdg_toplevel_close,
	.configure = __xdg_toplevel_configure,
	.configure_bounds = __xdg_toplevel_configure_bounds,
	.wm_capabilities = __xdg_toplevel_wm_capabilities
};





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createWindow(t_window *win, const size_t w, const size_t h, const char *t) {
	*win = (t_window) calloc(1, sizeof(struct s_window));

	assert(__gf_connectDisplay(*win));
	
	gf_setWindowResizable(*win, false);
	gf_setWindowBorderless(*win, false);
	gf_setWindowFullscreen(*win, false);

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

#if defined (VERBOSE)
	gf_logi("DISPLAY: Connected successfully\n");
#endif

	win->wl.reg = wl_display_get_registry(win->wl.dsp);
	if (!win->wl.reg) {

#if defined (VERBOSE)
		gf_loge("REGISTRY: Failed to get\n");
#endif

		return (false);
	}

#if defined (VERBOSE)
	gf_logi("REGISTRY: Found successfully\n");
#endif
	return (true);
}



/* * Wayland configuration functions
 * * */

GFAPIS void __wl_registry_global(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version) {

}

GFAPIS void	__wl_registry_global_remove(void *data, struct wl_registry *wl_registry, uint32_t name) {

}



GFAPIS void	__xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {

}



GFAPIS void	__xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {

}



GFAPIS void	__xdg_toplevel_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {

}

GFAPIS void	__xdg_toplevel_close(void *data, struct xdg_toplevel *xdg_toplevel) {

}

GFAPIS void	__xdg_toplevel_configure_bounds(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height) {

}

GFAPIS void	__xdg_toplevel_wm_capabilities(void *data, struct xdg_toplevel *xdg_toplevel, struct wl_array *capabilities) {

}
