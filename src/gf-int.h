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
# elif defined (USE_GLX)
#  include <GL/glx.h>
#  include <GL/glxext.h>
#endif

# if defined (USE_X11)
#  include <X11/X.h>
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#  include <X11/XKBlib.h>
#  include <X11/keysym.h>
#  include <X11/keysymdef.h>
#  define _NET_WM_STATE_REMOVE      0    /* remove/unset property */
#  define _NET_WM_STATE_ADD         1    /* add/set property */
#  define _NET_WM_STATE_TOGGLE      2    /* toggle property  */
# elif defined (USE_WL)

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

	} wl;
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

# include <dlfcn.h>
# include <string.h>
# include <limits.h>
# include <assert.h>
# include <stdlib.h>
#endif
