#if !defined USE_WL && !defined USE_EGL
# define USE_WL
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_connectDisplay(t_window);
GFAPIS bool	__gf_createSurface(t_window);
GFAPIS bool	__gf_createToplevel(t_window);



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
	assert(__gf_createSurface(*win));
	assert(__gf_createToplevel(*win));

	(*win)->wl.id = wl_egl_window_create((*win)->wl.surf, w, h);
	if (!(*win)->wl.id) {

#if defined (VERBOSE)
		gf_loge("WINDOW: Failed to create\n");
#endif

		return (false);
	}
	
	if (!t) {
		xdg_toplevel_set_title((*win)->xdg.toplevel, t);

#if defined (VERBOSE)
		gf_logi("WINDOW: Title set: %s\n", t);
#endif

	}
	wl_surface_commit((*win)->wl.surf);

#if defined (VERBOSE)
	gf_logi("WINDOW: Created successfully\n");
#endif

	/* Get the misc. data
	 * */
	assert(gf_getWindowSize(*win, 0, 0));
	assert(gf_getWindowPosition(*win, 0, 0));

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

	wl_registry_add_listener(win->wl.reg, &g_registry_listener, win);
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
	win->xdg.surf = xdg_wm_base_get_xdg_surface(win->xdg.base, win->wl.surf);
	if (!win->xdg.surf) {

#if defined (VERBOSE)
		gf_loge("XDG: Failed to create a surface\n");
#endif

		return (false);
	}
	xdg_surface_add_listener(win->xdg.surf, &g_surface_xdg_listener, win);

#if defined (VERBOSE)
	gf_logi("XDG: Surface created successfully\n");
#endif

	return (true);
}

GFAPIS bool	__gf_createToplevel(t_window win) {
	win->xdg.toplevel = xdg_surface_get_toplevel(win->xdg.surf);
	if (!win->xdg.toplevel) {

#if defined (VERBOSE)
		gf_loge("XDG: Failed to create a toplevel\n");
#endif

		return (false);
	}
	xdg_toplevel_add_listener(win->xdg.toplevel, &g_toplevel_listener, win);

#if defined (VERBOSE)
	gf_logi("XDG: Toplevel created successfully\n");
#endif

	return (true);
}



/* * Wayland configuration functions
 * * */

GFAPIS void __wl_registry_global(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version) {
	t_window	_win;

	(void) version;
	_win = (t_window) data;
	if (!strcmp(interface, wl_compositor_interface.name)) {
		_win->wl.comp = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);

#if defined (VERBOSE)
		gf_logi("REGISTRY: Bound compositor interface\n");
#endif

	}

	if (!strcmp(interface, xdg_wm_base_interface.name)) {
		_win->xdg.base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
		xdg_wm_base_add_listener(_win->xdg.base, &g_wm_base_listener, data);

#if defined (VERBOSE)
		gf_logi("REGISTRY: Bound xdg window manager's base interface\n");
#endif

	}
}

GFAPIS void	__wl_registry_global_remove(void *data, struct wl_registry *wl_registry, uint32_t name) {
	(void) data;
	(void) wl_registry;
	(void) name;
}



GFAPIS void	__xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
	(void) data;
	xdg_wm_base_pong(xdg_wm_base, serial);
}



GFAPIS void	__xdg_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
	(void) data;
	xdg_surface_ack_configure(xdg_surface, serial);
}



GFAPIS void	__xdg_toplevel_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
	t_window	_win;
	t_event		_event;

	(void) xdg_toplevel;
	(void) states;
	_win = (t_window) data;

	/* Resize the window itself
	 * */
	_win->misc.width = width;
	_win->misc.height = height;
	wl_egl_window_resize(_win->wl.id, width, height, 0, 0);

	/* Push the resize event
	 * */
	_event.type = GF_EVENT_RESIZE;
	_event.resize.w = width;
	_event.resize.h = height;
	gf_pushEvent(_win, &_event);
}

GFAPIS void	__xdg_toplevel_close(void *data, struct xdg_toplevel *xdg_toplevel) {
	t_event	_event;

	(void) xdg_toplevel;
	_event.type = _event.quit.type = GF_EVENT_QUIT;
	gf_pushEvent(data, &_event);
}

GFAPIS void	__xdg_toplevel_configure_bounds(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height) {	
	(void) data;
	(void) xdg_toplevel;
	(void) width;
	(void) height;
}

GFAPIS void	__xdg_toplevel_wm_capabilities(void *data, struct xdg_toplevel *xdg_toplevel, struct wl_array *capabilities) {
	(void) data;
	(void) xdg_toplevel;
	(void) capabilities;
}
