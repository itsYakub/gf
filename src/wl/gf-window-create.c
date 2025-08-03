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

GFAPIS void __wl_seat_capabilities(void *, struct wl_seat *, uint32_t);
GFAPIS void __wl_seat_name(void *, struct wl_seat *, const char *);
	
GFAPIS void	__wl_keyboard_keymap(void *, struct wl_keyboard *, uint32_t, int32_t, uint32_t);
GFAPIS void	__wl_keyboard_enter(void *, struct wl_keyboard *, uint32_t, struct wl_surface *, struct wl_array *);
GFAPIS void	__wl_keyboard_leave(void *, struct wl_keyboard *, uint32_t, struct wl_surface *);
GFAPIS void	__wl_keyboard_key(void *, struct wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t);
GFAPIS void	__wl_keyboard_modifiers(void *, struct wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
GFAPIS void	__wl_keyboard_repeat_info(void *, struct wl_keyboard *, int32_t, int32_t);

GFAPIS void	__wl_pointer_enter(void *, struct wl_pointer *, uint32_t, struct wl_surface *, wl_fixed_t, wl_fixed_t);
GFAPIS void	__wl_pointer_leave(void *, struct wl_pointer *, uint32_t, struct wl_surface *);
GFAPIS void	__wl_pointer_motion(void *, struct wl_pointer *, uint32_t, wl_fixed_t, wl_fixed_t);
GFAPIS void	__wl_pointer_button(void *, struct wl_pointer *, uint32_t, uint32_t, uint32_t, uint32_t);
GFAPIS void	__wl_pointer_axis(void *, struct wl_pointer *, uint32_t, uint32_t, wl_fixed_t);
GFAPIS void	__wl_pointer_frame(void *, struct wl_pointer *); 
GFAPIS void	__wl_pointer_axis_source(void *, struct wl_pointer *, uint32_t);
GFAPIS void	__wl_pointer_axis_stop(void *, struct wl_pointer *, uint32_t, uint32_t);
GFAPIS void	__wl_pointer_axis_discrete(void *, struct wl_pointer *, uint32_t, int32_t);
GFAPIS void	__wl_pointer_axis_value120(void *, struct wl_pointer *, uint32_t, int32_t);





/* SECTION:
 *  Static globals
 * */

static struct wl_registry_listener	g_registry_listener = {
	.global = __wl_registry_global,
	.global_remove = __wl_registry_global_remove
};

static struct wl_seat_listener		g_seat_listener = {
	.capabilities = __wl_seat_capabilities,
	.name = __wl_seat_name
};

static struct wl_keyboard_listener	g_keyboard_listener = {
	.keymap = __wl_keyboard_keymap,
	.enter = __wl_keyboard_enter,
	.leave = __wl_keyboard_leave,
	.key = __wl_keyboard_key,
	.modifiers = __wl_keyboard_modifiers,
	.repeat_info = __wl_keyboard_repeat_info
};

static struct wl_pointer_listener	g_pointer_listener = {
	.enter = __wl_pointer_enter,
	.leave = __wl_pointer_leave,
	.motion = __wl_pointer_motion,
	.button = __wl_pointer_button,
	.axis = __wl_pointer_axis,
	.frame = __wl_pointer_frame,
	.axis_source = __wl_pointer_axis_source,
	.axis_stop = __wl_pointer_axis_stop,
	.axis_discrete = __wl_pointer_axis_discrete,
	.axis_value120 = __wl_pointer_axis_value120,
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
	
	if (t) {
		xdg_toplevel_set_title((*win)->xdg.toplevel, t);

#if defined (VERBOSE)
		gf_logi("WINDOW: Title set: %s\n", t);
#endif

	}
	wl_surface_commit((*win)->wl.surf);

#if defined (VERBOSE)
	gf_logi("WINDOW: Created successfully\n");
#endif

	/* Another roundtrip to create all the remaining structures, especially wl_seat
	 * */
	wl_display_roundtrip((*win)->wl.dsp);

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
	
	if (!strcmp(interface, wl_seat_interface.name)) {
		_win->wl.seat = wl_registry_bind(wl_registry, name, &wl_seat_interface, 1);
		wl_seat_add_listener(_win->wl.seat, &g_seat_listener, data);

#if defined (VERBOSE)
		gf_logi("REGISTRY: Bound seat interface\n");
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



GFAPIS void __wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities) {
	t_window	_win;

	_win = (t_window) data;
	if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD && !_win->wl.keyboard) {
		_win->wl.keyboard = wl_seat_get_keyboard(wl_seat);
		_win->xkb.ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
		wl_keyboard_add_listener(_win->wl.keyboard, &g_keyboard_listener, data);

#if defined (VERBOSE)
		gf_logi("KEYBOARD: Created successfully\n");
#endif

	}

	if (capabilities & WL_SEAT_CAPABILITY_POINTER && !_win->wl.pointer) {
		_win->wl.pointer = wl_seat_get_pointer(wl_seat);
		wl_pointer_add_listener(_win->wl.pointer, &g_pointer_listener, data);

#if defined (VERBOSE)
		gf_logi("POINTER: Created successfully\n");
#endif

	}
}

GFAPIS void __wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name) {
	(void) data;
	(void) wl_seat;
	(void) name;
}



GFAPIS void	__wl_keyboard_keymap(void *data, struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {
	t_window	_win;
	char		*_str;

	(void) wl_keyboard;
	(void) format;
	(void) fd;

	_win = (t_window) data;
	_str = 0;
	_str = mmap(_str, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (_str == MAP_FAILED) {

#if defined (VERBOSE)
		gf_loge("XKB: Shared-memory allocation failed\n");
#endif

		return;
	}

	_win->xkb.keymap = xkb_keymap_new_from_string(_win->xkb.ctx, _str, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	if (!_win->xkb.keymap) {

#if defined (VERBOSE)
		gf_loge("XKB: Failed to create keymap\n");
#endif

		return;
	}

#if defined (VERBOSE)
	gf_logi("XKB: Keymap created successfully\n");
#endif

	munmap(_str, size);
	close(fd);

	_win->xkb.state = xkb_state_new(_win->xkb.keymap);
	if (!_win->xkb.state) {

#if defined (VERBOSE)
		gf_loge("XKB: Failed to create a State Machine\n");
#endif

		return;
	}

#if defined (VERBOSE)
	gf_logi("XKB: State Machine created successfully\n");
#endif

}

GFAPIS void	__wl_keyboard_enter(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface, struct wl_array *keys) {
	(void) data;
	(void) wl_keyboard;
	(void) serial;
	(void) surface;
	(void) keys;
}

GFAPIS void	__wl_keyboard_leave(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface) {
	(void) data;
	(void) wl_keyboard;
	(void) serial;
	(void) surface;
}

GFAPIS void	__wl_keyboard_key(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
	xkb_keysym_t	_keysym;
	t_window		_win;
	t_event			_event;

	(void) wl_keyboard;
	(void) serial;
	(void) time;

	_win = (t_window) data;
	_event.type = state ? GF_EVENT_KEY_PRESS : GF_EVENT_KEY_RELEASE;
	_keysym = xkb_state_key_get_one_sym(_win->xkb.state, key + 8);
	_event.key.key = _gf_keymapPlatformToGf(_keysym);
	_event.key.state = state;
	gf_pushEvent(data, &_event);	
}

GFAPIS void	__wl_keyboard_modifiers(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
	(void) data;
	(void) wl_keyboard;
	(void) serial;
	(void) mods_depressed;
	(void) mods_latched;
	(void) mods_locked;
	(void) group;
	/* TODO(yakub):
	 *  Current key press can be modified by modifier keys
	 *  Implement that as a modification to the main event (maybe stored in the 'data')
	 * */
}

GFAPIS void	__wl_keyboard_repeat_info(void *data, struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay) {
	(void) data;
	(void) wl_keyboard;
	(void) rate;
	(void) delay;
}



GFAPIS void	__wl_pointer_enter(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
	(void) data;
	(void) wl_pointer;
	(void) serial;
	(void) surface;
	(void) surface_x;
	(void) surface_y;
	/* TODO(yakub):
	 *  Implement focus event
	 * */
}

GFAPIS void	__wl_pointer_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface) {
	(void) data;
	(void) wl_pointer;
	(void) serial;
	(void) surface;
	/* TODO(yakub):
	 *  Implement focus event
	 * */
}

GFAPIS void	__wl_pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
	t_event	_event;

	(void) wl_pointer;
	(void) time;
	_event.type = GF_EVENT_MOUSEMOTION;
	/* TODO(yakub):
	 *  Get different values for position and relative position
	 * */
	_event.motion.x = _event.motion.xrel = surface_x;
	_event.motion.y = _event.motion.yrel = surface_y;
	gf_pushEvent(data, &_event);
}

GFAPIS void	__wl_pointer_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
	t_event	_event;

	(void) data;
	(void) wl_pointer;
	(void) serial;
	(void) time;
	_event.type = state ? GF_EVENT_MOUSE_PRESS : GF_EVENT_MOUSE_RELEASE;
	_event.key.key = _gf_buttonPlatformToGf(button);
	_event.key.state = state;
	gf_pushEvent(data, &_event);	
}

GFAPIS void	__wl_pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
	(void) data;
	(void) wl_pointer;
	(void) time;
	(void) axis;
	(void) value;
}

GFAPIS void	__wl_pointer_frame(void *data, struct wl_pointer *wl_pointer) {
	(void) data;
	(void) wl_pointer;
}

GFAPIS void	__wl_pointer_axis_source(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source) {
	(void) data;
	(void) wl_pointer;
	(void) axis_source;
}

GFAPIS void	__wl_pointer_axis_stop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {
	(void) data;
	(void) wl_pointer;
	(void) time;
	(void) axis;
}

GFAPIS void	__wl_pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {
	(void) data;
	(void) wl_pointer;
	(void) axis;
	(void) discrete;
}

GFAPIS void	__wl_pointer_axis_value120(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t value120) {
	(void) data;
	(void) wl_pointer;
	(void) axis;
	(void) value120;
}
