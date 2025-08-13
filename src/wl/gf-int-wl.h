#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_wl_h_)
# define _gf_int_wl_h_

# if !defined (USE_WL)
#  define USE_WL
# endif
# include "./../gf.h"
# include <wayland-client.h>
# include <wayland-client-core.h>
# include <wayland-client-protocol.h>
# include <wayland-egl.h>
# include <wayland-egl-core.h>
# include <xkbcommon/xkbcommon.h>
# include "./wl/xdg-shell.h"

/* SECTION:
 *  Type definitions
 * */

struct s_client {
	struct {
		struct wl_display		*dsp;
		struct wl_egl_window	*id;
		struct wl_registry		*reg;
		struct wl_compositor	*comp;
		struct wl_surface		*surf;

		struct wl_seat			*seat;
		struct wl_keyboard		*keyboard;
		struct wl_pointer		*pointer;
	} wl;

	struct {
		struct wl_registry_listener		registry;
		struct wl_seat_listener			seat;
		struct wl_keyboard_listener		keyboard;
		struct wl_pointer_listener		pointer;
		struct xdg_wm_base_listener		wm_base;
		struct xdg_surface_listener		surface;
		struct xdg_toplevel_listener	toplevel;
	} listener;
	
	struct {
		struct xdg_wm_base		*base;
		struct xdg_surface		*surf;
		struct xdg_toplevel		*toplevel;
	} xdg;

	struct {
		struct xkb_context		*ctx;
		struct xkb_keymap		*keymap;
		struct xkb_state		*state;
	} xkb;
};

#endif
