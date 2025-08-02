#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_h_)
# define _gf_int_h_

# include "./gf.h"
# define _GF_EVENT_QUEUE_SIZE	64

# if defined (USE_EGL)
#  include <EGL/egl.h>
#  include <EGL/eglext.h>
#  if defined (USE_WL)
#   include <wayland-egl.h>
#   include <wayland-egl-core.h>
#  endif
# elif defined (USE_GLX)
#  include <GL/glx.h>
#  include <GL/glxext.h>
# endif
# include <GL/gl.h>

# if defined (USE_X11)
#  include <X11/X.h>
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#  include <X11/XKBlib.h>
#  include <X11/keysym.h>
#  include <X11/keysymdef.h>
#  define _NET_WM_STATE_REMOVE	0	/* remove/unset property */
#  define _NET_WM_STATE_ADD		1	/* add/set property */
#  define _NET_WM_STATE_TOGGLE	2	/* toggle property  */
# elif defined (USE_WL)
#  include <wayland-client.h>
#  include <wayland-client-core.h>
#  include <wayland-client-protocol.h>

/* This client needs to be generated either by the build system or manually in the ./src/wl directory:
 * $ wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml /path/to/gf/src/wl/xdg-shell.h
 *
 * REMEBER: We also need to generate a source code for the xdg library, preferably at the same location as xdg-shell.h
 * $ wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml /path/to/gf/src/wl/xdg-shell.c
 * */
#  include "./wl/xdg-shell.h"
# endif

struct s_window {

# if defined (USE_X11)
	struct {
		Display		*dsp;
		XVisualInfo	*info;
		Window		id;
		Window		root_id;
		int32_t		screen_id;
	} x11;
	
	struct {
		Atom	wm_delete_window;
		Atom	wm_net_state;
		Atom	wm_net_state_above;
		Atom	wm_net_state_fullscreen;
		Atom	wm_net_state_hidden;
		Atom	wm_net_state_maximized_horz;
		Atom	wm_net_state_maximized_vert;
		Atom	wm_net_window_type;
		Atom	wm_net_window_normal;
		Atom	wm_net_window_dock;
	} atoms;
# elif defined (USE_WL)
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
		struct xdg_wm_base		*base;
		struct xdg_surface		*surf;
		struct xdg_toplevel		*toplevel;
	} xdg;
# endif

# if defined (USE_EGL)
	struct {
		EGLDisplay	dsp;
		EGLConfig	conf;
		EGLSurface	surf;
		EGLContext	ctx;
	} egl;
# elif defined (USE_GLX)
	struct {
		GLXFBConfig	conf;
		GLXContext	ctx;
	} glx;
# endif

	struct {
		t_event	lst[_GF_EVENT_QUEUE_SIZE];
		size_t	cnt;
	} events;

	struct {
		int32_t	width;
		int32_t	height;
		int32_t	x;
		int32_t	y;
	} misc;
};

typedef struct s_window	*t_window;

# include <errno.h>
# include <string.h>
# include <limits.h>
# include <assert.h>
# include <stdlib.h>
# if defined (__linux__)
#  include <poll.h>
#  include <dlfcn.h>
# endif
#endif
