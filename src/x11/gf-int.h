#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_h_)
# define _gf_int_h_
# define GF_INT_EVENT_QUEUE_SIZE	64

# define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
# define _NET_WM_STATE_ADD           1    /* add/set property */
# define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

#include "./../gf.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GL/glx.h>
#include <GL/glxext.h>

struct s_window {
	
	struct {
		Display		*dsp;
		XVisualInfo	*info;
		Window		id;
		Window		root_id;
		int32_t		screen_id;
	} x11;
	
	struct {
		EGLDisplay	dsp;
		EGLConfig	conf;
		EGLSurface	surf;
		EGLContext	ctx;
	} egl;

	struct {
		GLXFBConfig	conf;
		GLXContext	ctx;
	} glx;
		
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

	struct {
		t_event	lst[GF_INT_EVENT_QUEUE_SIZE];
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

# if !defined gf_int_ensureWindow
#  include <assert.h>
#  define gf_int_ensureWindow(w) assert(w->x11.dsp && w->x11.id != None)
# endif

			

#endif
