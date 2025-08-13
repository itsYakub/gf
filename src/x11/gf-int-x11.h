#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_x11_h_)
# define _gf_int_x11_h_

# if !defined USE_X11
#  define USE_X11
# endif
# include "./../gf.h"
# include <X11/X.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/Xatom.h>
# include <X11/XKBlib.h>
# include <X11/keysym.h>
# include <X11/keysymdef.h>
# define _NET_WM_STATE_REMOVE	0	/* remove/unset property */
# define _NET_WM_STATE_ADD		1	/* add/set property */
# define _NET_WM_STATE_TOGGLE	2	/* toggle property  */

/* SECTION:
 *  Type definitions
 * */

struct s_client {
	Display		*dsp;
	XVisualInfo	*info;
	Window		id;
	Window		root_id;
	int32_t		screen_id;
	
	struct {
		Atom	WM_DELETE_WINDOW;
		Atom	WM_CHANGE_STATE;

		Atom	_MOTIF_WM_HINTS;

		Atom	_NET_WM_STATE;
		Atom	_NET_WM_STATE_ABOVE;
		Atom	_NET_WM_STATE_FULLSCREEN;
		Atom	_NET_WM_STATE_HIDDEN;
		Atom	_NET_WM_STATE_MAXIMIZED_HORZ;
		Atom	_NET_WM_STATE_MAXIMIZED_VERT;
		Atom	_NET_WM_WINDOW_TYPE;
		Atom	_NET_WM_WINDOW_NORMAL;
		Atom	_NET_WM_WINDOW_DOCK;
	} atoms;
};

/* SECTION:
 *  Event Interface
 * */

GFAPII bool	gf_int_pollInternal_Client(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Property(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Configure(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Mouse(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Key(t_window, XEvent *);

GFAPII bool	gf_int_updateWindowConfig(t_window);	

#endif
