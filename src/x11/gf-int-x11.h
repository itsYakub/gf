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

typedef Display		*(*PFN_XOpenDisplay) (const char *);
typedef int			(*PFN_XCloseDisplay) (Display *);
typedef Status		(*PFN_XMatchVisualInfo) (Display *, int, int, int, XVisualInfo *);
typedef Colormap	(*PFN_XCreateColormap) (Display *, Window, Visual *, int);
typedef Window		(*PFN_XCreateWindow) (Display *, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual *, unsigned long, XSetWindowAttributes *);
typedef int			(*PFN_XSetWMHints) (Display *, Window, XWMHints *);
typedef int			(*PFN_XSelectInput) (Display *, Window, long);
typedef int			(*PFN_XMapWindow) (Display *, Window);
typedef int			(*PFN_XDestroyWindow) (Display *, Window);
typedef int			(*PFN_XUnmapWindow) (Display *, Window);
typedef Atom		(*PFN_XInternAtom) (Display *, const char *, Bool);
typedef Status		(*PFN_XSetWMProtocols) (Display *, Window, Atom *, int);
typedef int			(*PFN_XPending) (Display *);
typedef int			(*PFN_XNextEvent) (Display *,XEvent *);
typedef Status		(*PFN_XSendEvent) (Display *, Window, Bool, long, XEvent *);
typedef int			(*PFN_XFlush) (Display *);
typedef int			(*PFN_XFree) (void *);
typedef int			(*PFN_XGetWindowProperty) (Display *, Window, Atom, long, long, Bool, Atom, Atom *, int *, unsigned long *, unsigned long *, unsigned char **);
typedef Status		(*PFN_XGetWindowAttributes) (Display *, Window, XWindowAttributes *);
typedef Bool		(*PFN_XTranslateCoordinates) (Display *, Window, Window, int, int, int *, int *, Window *);
typedef int			(*PFN_XResizeWindow) (Display *, Window, unsigned int, unsigned int);
typedef int			(*PFN_XMoveWindow) (Display *, Window, int, int);
typedef int			(*PFN_XStoreName) (Display *, Window, const char *);
typedef Status		(*PFN_XGetWMNormalHints) (Display *, Window, XSizeHints *, long *);
typedef void		(*PFN_XSetWMNormalHints) (Display *, Window, XSizeHints *);
typedef int			(*PFN_XChangeProperty) (Display *, Window, Atom, Atom, int, int, const unsigned char *, int);
typedef KeySym		(*PFN_XkbKeycodeToKeysym) (Display *, KeyCode, int, int);

struct s_X11 {
	void	*handle;
	struct {
		PFN_XOpenDisplay			XOpenDisplay;
		PFN_XCloseDisplay			XCloseDisplay;
		PFN_XMatchVisualInfo		XMatchVisualInfo;
		PFN_XCreateColormap			XCreateColormap;
		PFN_XCreateWindow			XCreateWindow;
		PFN_XSetWMHints				XSetWMHints;
		PFN_XSelectInput			XSelectInput;
		PFN_XMapWindow				XMapWindow;
		PFN_XDestroyWindow			XDestroyWindow;
		PFN_XUnmapWindow			XUnmapWindow;
		PFN_XInternAtom				XInternAtom;
		PFN_XSetWMProtocols			XSetWMProtocols;
		PFN_XPending				XPending;
		PFN_XNextEvent				XNextEvent;
		PFN_XSendEvent				XSendEvent;
		PFN_XFlush					XFlush;
		PFN_XFree					XFree;
		PFN_XGetWindowProperty		XGetWindowProperty;
		PFN_XGetWindowAttributes	XGetWindowAttributes;
		PFN_XTranslateCoordinates	XTranslateCoordinates;
		PFN_XResizeWindow			XResizeWindow;
		PFN_XMoveWindow				XMoveWindow;
		PFN_XStoreName				XStoreName;
		PFN_XGetWMNormalHints		XGetWMNormalHints;
		PFN_XSetWMNormalHints		XSetWMNormalHints;
		PFN_XChangeProperty			XChangeProperty;
		PFN_XkbKeycodeToKeysym		XkbKeycodeToKeysym;
	} /* X11 API */;
};

extern struct s_X11	g_X11;

/* SECTION:
 *  Internal function declarations
 * */

GFAPII bool	gf_int_initPlatformX11(void);

GFAPII bool	gf_int_pollInternal_Client(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Property(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Configure(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Mouse(t_window, XEvent *);
GFAPII bool	gf_int_pollInternal_Key(t_window, XEvent *);

GFAPII bool	gf_int_updateWindowConfig(t_window);	

#endif
