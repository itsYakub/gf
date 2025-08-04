#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_wl_h_)
# define _gf_int_wl_h_

# if !defined (USE_WL)
#  define USE_WL
# endif
# include "./../gf-int.h"
# include "./../gf.h"

/* SECTION:
 *  Listener Interface
 * */

/* Function responsible for setting up every listener struct of the client
 * NOTE(yakub):
 *  That doesn't mean it handles binding them as this would be too complex and can be easily handled inside listeners itself or other window creation functions
 * */
GFAPII bool	gf_int_initListeners(t_window);

/* NOTE(yakub):
 *  This is a list of signatures of every listener functions we're using for windowing.
 *  Ranging from configuration to event processing.
 *  - Registry - adds another components of the wayland server interface to our client (compositors, bases, seats etc.)
 *  - wm_base - basic ping-pong communication between client and server
 *  - Surface - sending ack_configure request for that surface
 *  - Toplevel - functions specific to window surface configuration (resizing, quitting etc.)
 *  - Seat - configuration of keyboard and mouse pointer
 *  - Keyboard - key input specific events (focus, press, release etc.)
 *  - Pointer - mouse pointer specific events (focus, motion, press, release etc.)
 * */

GFAPII void gf_int_registry_global(void *, struct wl_registry *, uint32_t, const char *, uint32_t);
GFAPII void	gf_int_registry_global_remove(void *, struct wl_registry *, uint32_t);

GFAPII void	gf_int_wm_base_ping(void *, struct xdg_wm_base *, uint32_t);

GFAPII void	gf_int_surface_configure(void *, struct xdg_surface *, uint32_t);

GFAPII void	gf_int_toplevel_configure(void *, struct xdg_toplevel *, int32_t, int32_t, struct wl_array *);
GFAPII void	gf_int_toplevel_close(void *, struct xdg_toplevel *);
GFAPII void	gf_int_toplevel_configure_bounds(void *, struct xdg_toplevel *, int32_t, int32_t);
GFAPII void	gf_int_toplevel_wm_capabilities(void *, struct xdg_toplevel *, struct wl_array *);

GFAPII void gf_int_seat_capabilities(void *, struct wl_seat *, uint32_t);
GFAPII void gf_int_seat_name(void *, struct wl_seat *, const char *);

GFAPII void	gf_int_keyboard_keymap(void *, struct wl_keyboard *, uint32_t, int32_t, uint32_t);
GFAPII void	gf_int_keyboard_enter(void *, struct wl_keyboard *, uint32_t, struct wl_surface *, struct wl_array *);
GFAPII void	gf_int_keyboard_leave(void *, struct wl_keyboard *, uint32_t, struct wl_surface *);
GFAPII void	gf_int_keyboard_key(void *, struct wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t);
GFAPII void	gf_int_keyboard_modifiers(void *, struct wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
GFAPII void	gf_int_keyboard_repeat_info(void *, struct wl_keyboard *, int32_t, int32_t);

GFAPII void	gf_int_pointer_enter(void *, struct wl_pointer *, uint32_t, struct wl_surface *, wl_fixed_t, wl_fixed_t);
GFAPII void	gf_int_pointer_leave(void *, struct wl_pointer *, uint32_t, struct wl_surface *);
GFAPII void	gf_int_pointer_motion(void *, struct wl_pointer *, uint32_t, wl_fixed_t, wl_fixed_t);
GFAPII void	gf_int_pointer_button(void *, struct wl_pointer *, uint32_t, uint32_t, uint32_t, uint32_t);
GFAPII void	gf_int_pointer_axis(void *, struct wl_pointer *, uint32_t, uint32_t, wl_fixed_t);
GFAPII void	gf_int_pointer_frame(void *, struct wl_pointer *); 
GFAPII void	gf_int_pointer_axis_source(void *, struct wl_pointer *, uint32_t);
GFAPII void	gf_int_pointer_axis_stop(void *, struct wl_pointer *, uint32_t, uint32_t);
GFAPII void	gf_int_pointer_axis_discrete(void *, struct wl_pointer *, uint32_t, int32_t);
GFAPII void	gf_int_pointer_axis_value120(void *, struct wl_pointer *, uint32_t, int32_t);

#endif
