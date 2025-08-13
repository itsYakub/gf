#if !defined (USE_WL) && !defined (USE_EGL)
# define USE_WL
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"
#include "./gf-int-wl.h"

GFAPII bool	gf_int_initListeners(t_window win) {
	win->client.listener.registry.global = gf_int_registry_global;
	win->client.listener.registry.global_remove = gf_int_registry_global_remove;

	win->client.listener.wm_base.ping = gf_int_wm_base_ping;
		
	win->client.listener.seat.name = gf_int_seat_name;
	win->client.listener.seat.capabilities = gf_int_seat_capabilities;
	
	win->client.listener.surface.configure = gf_int_surface_configure;
	
	win->client.listener.toplevel = (struct xdg_toplevel_listener) {
		gf_int_toplevel_configure,
		gf_int_toplevel_close,
		gf_int_toplevel_configure_bounds,
		gf_int_toplevel_wm_capabilities,
	};

	win->client.listener.keyboard = (struct wl_keyboard_listener) {
		gf_int_keyboard_keymap, gf_int_keyboard_enter, gf_int_keyboard_leave,
		gf_int_keyboard_key, gf_int_keyboard_modifiers, gf_int_keyboard_repeat_info,
	};
	
	win->client.listener.pointer = (struct wl_pointer_listener) {
		gf_int_pointer_enter, gf_int_pointer_leave,
		gf_int_pointer_motion, gf_int_pointer_button,
		gf_int_pointer_axis, gf_int_pointer_frame,
		gf_int_pointer_axis_source, gf_int_pointer_axis_stop,
		gf_int_pointer_axis_discrete, gf_int_pointer_axis_value120,
	};
	
	return (true);
}



/* SECTION:
 *  Registry listeners
 * */

GFAPII void gf_int_registry_global(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version) {
	t_window	_win;

	(void) version;
	_win = (t_window) data;
	if (!strcmp(interface, wl_compositor_interface.name)) {
		_win->client.comp = wl_registry_bind(wl_registry, name, &wl_compositor_interface, 4);
	}

	if (!strcmp(interface, xdg_wm_base_interface.name)) {
		_win->xdg.base = wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, 1);
		xdg_wm_base_add_listener(_win->xdg.base, &_win->client.listener.wm_base, data);
	}
	
	if (!strcmp(interface, wl_seat_interface.name)) {
		_win->client.seat = wl_registry_bind(wl_registry, name, &wl_seat_interface, 1);

		wl_seat_add_listener(_win->client.seat, &_win->client.listener.seat, data);
	}
}

GFAPII void	gf_int_registry_global_remove(void *data, struct wl_registry *wl_registry, uint32_t name) {
	(void) data;
	(void) wl_registry;
	(void) name;
}



/* SECTION:
 *  wm_base listeners
 * */

GFAPII void	gf_int_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
	(void) data;
	xdg_wm_base_pong(xdg_wm_base, serial);
}



/* SECTION:
 *  Surface listeners
 * */

GFAPII void	gf_int_surface_configure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
	(void) data;
	xdg_surface_ack_configure(xdg_surface, serial);
}



/* SECTION:
 *  Toplevel listeners
 * */

GFAPII void	gf_int_toplevel_configure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height, struct wl_array *states) {
	t_event		_event;

	(void) data;
	(void) xdg_toplevel;
	(void) states;

	/* Resize the window itself
	 * */
	gf_setWindowSize(data, width, height);

	/* Push the resize event
	 * */
	_event.type = GF_EVENT_RESIZE;
	_event.resize.w = width;
	_event.resize.h = height;
	gf_pushEvent(data, &_event);
}

GFAPII void	gf_int_toplevel_close(void *data, struct xdg_toplevel *xdg_toplevel) {
	t_event	_event;

	(void) xdg_toplevel;
	_event.type = _event.quit.type = GF_EVENT_QUIT;
	gf_pushEvent(data, &_event);
}

GFAPII void	gf_int_toplevel_configure_bounds(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height) {	
	(void) data;
	(void) xdg_toplevel;
	(void) width;
	(void) height;
}

GFAPII void	gf_int_toplevel_wm_capabilities(void *data, struct xdg_toplevel *xdg_toplevel, struct wl_array *capabilities) {
	(void) data;
	(void) xdg_toplevel;
	(void) capabilities;
}



/* SECTION:
 *  Seat listeners
 * */

GFAPII void gf_int_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities) {
	t_window	_win;

	_win = (t_window) data;
	if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD && !_win->client.keyboard) {
		_win->client.keyboard = wl_seat_get_keyboard(wl_seat);
		_win->client.xkb.ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
		wl_keyboard_add_listener(_win->client.keyboard, &_win->client.listener.keyboard, data);
	}

	if (capabilities & WL_SEAT_CAPABILITY_POINTER && !_win->client.pointer) {
		_win->client.pointer = wl_seat_get_pointer(wl_seat);
		wl_pointer_add_listener(_win->client.pointer, &_win->client.listener.pointer, data);
	}
}

GFAPII void gf_int_seat_name(void *data, struct wl_seat *wl_seat, const char *name) {
	(void) data;
	(void) wl_seat;
	(void) name;
}



/* SECTION:
 *  Keyboard listeners
 * */

GFAPII void	gf_int_keyboard_keymap(void *data, struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {
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

	_win->client.xkb.keymap = xkb_keymap_new_from_string(_win->client.xkb.ctx, _str, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
	if (!_win->client.xkb.keymap) {

#if defined (VERBOSE)
		gf_loge("XKB: Failed to create keymap\n");
#endif

		return;
	}
	munmap(_str, size);
	close(fd);

	_win->client.xkb.state = xkb_state_new(_win->client.xkb.keymap);
	if (!_win->client.xkb.state) {

#if defined (VERBOSE)
		gf_loge("XKB: Failed to create a State Machine\n");
#endif

		return;
	}
}

GFAPII void	gf_int_keyboard_enter(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface, struct wl_array *keys) {
	(void) data;
	(void) wl_keyboard;
	(void) serial;
	(void) surface;
	(void) keys;
}

GFAPII void	gf_int_keyboard_leave(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *surface) {
	(void) data;
	(void) wl_keyboard;
	(void) serial;
	(void) surface;
}

GFAPII void	gf_int_keyboard_key(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
	xkb_keysym_t	_keysym;
	t_window		_win;
	t_event			_event;

	(void) wl_keyboard;
	(void) serial;
	(void) time;

	_win = (t_window) data;
	_event.type = state ? GF_EVENT_KEY_PRESS : GF_EVENT_KEY_RELEASE;
	_keysym = xkb_state_key_get_one_sym(_win->client.xkb.state, key + 8);
	_event.key.key = gf_int_keymapPlatformToGf(_keysym);
	_event.key.state = state;
	gf_pushEvent(data, &_event);	
}

GFAPII void	gf_int_keyboard_modifiers(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {
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

GFAPII void	gf_int_keyboard_repeat_info(void *data, struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay) {
	(void) data;
	(void) wl_keyboard;
	(void) rate;
	(void) delay;
}



/* SECTION:
 *  Pointer listeners
 * */

GFAPII void	gf_int_pointer_enter(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x, wl_fixed_t surface_y) {
	t_event	_event;

	(void) wl_pointer;
	(void) serial;
	(void) surface;
	(void) surface_x;
	(void) surface_y;
	_event.type = GF_EVENT_MOUSE_FOCUS;
	_event.focus.state = true;
	gf_pushEvent(data, &_event);
}

GFAPII void	gf_int_pointer_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface) {
	t_event	_event;

	(void) wl_pointer;
	(void) serial;
	(void) surface;
	_event.type = GF_EVENT_MOUSE_FOCUS;
	_event.focus.state = false;
	gf_pushEvent(data, &_event);
}

GFAPII void	gf_int_pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
	t_event		_event;

	(void) data;
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

GFAPII void	gf_int_pointer_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
	t_event	_event;

	(void) data;
	(void) wl_pointer;
	(void) serial;
	(void) time;
	_event.type = state ? GF_EVENT_MOUSE_PRESS : GF_EVENT_MOUSE_RELEASE;
	_event.key.key = gf_int_buttonPlatformToGf(button);
	_event.key.state = state;
	gf_pushEvent(data, &_event);	
}

GFAPII void	gf_int_pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
	(void) data;
	(void) wl_pointer;
	(void) time;
	(void) axis;
	(void) value;
}

GFAPII void	gf_int_pointer_frame(void *data, struct wl_pointer *wl_pointer) {
	(void) data;
	(void) wl_pointer;
}

GFAPII void	gf_int_pointer_axis_source(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source) {
	(void) data;
	(void) wl_pointer;
	(void) axis_source;
}

GFAPII void	gf_int_pointer_axis_stop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {
	(void) data;
	(void) wl_pointer;
	(void) time;
	(void) axis;
}

GFAPII void	gf_int_pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {
	(void) data;
	(void) wl_pointer;
	(void) axis;
	(void) discrete;
}

GFAPII void	gf_int_pointer_axis_value120(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t value120) {
	(void) data;
	(void) wl_pointer;
	(void) axis;
	(void) value120;
}
