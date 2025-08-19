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

/* SECTION: Xlib.h
 * */
typedef Display		*(*PFN_XOpenDisplay) (const char *);
typedef int32_t		(*PFN_XCloseDisplay) (Display *);
typedef Colormap	(*PFN_XCreateColormap) (Display *, Window, Visual *, int32_t);
typedef Window		(*PFN_XCreateWindow) (Display *, Window, int32_t, int32_t, uint32_t, uint32_t, uint32_t, int32_t, uint32_t, Visual *, unsigned long, XSetWindowAttributes *);
typedef int32_t		(*PFN_XSelectInput) (Display *, Window, long);
typedef int32_t		(*PFN_XMapWindow) (Display *, Window);
typedef int32_t		(*PFN_XDestroyWindow) (Display *, Window);
typedef int32_t		(*PFN_XUnmapWindow) (Display *, Window);
typedef int32_t		(*PFN_XPending) (Display *);
typedef int32_t		(*PFN_XNextEvent) (Display *, XEvent *);
typedef Status		(*PFN_XSendEvent) (Display *, Window, Bool, long, XEvent *);
typedef int32_t		(*PFN_XFlush) (Display *);
typedef int32_t		(*PFN_XFree) (void *);
typedef int32_t		(*PFN_XGetWindowProperty) (Display *, Window, Atom, long, long, Bool, Atom, Atom *, int32_t *, unsigned long *, unsigned long *, unsigned char **);
typedef Status		(*PFN_XGetWindowAttributes) (Display *, Window, XWindowAttributes *);
typedef Bool		(*PFN_XTranslateCoordinates) (Display *, Window, Window, int32_t, int32_t, int32_t *, int32_t *, Window *);
typedef int32_t		(*PFN_XResizeWindow) (Display *, Window, uint32_t, uint32_t);
typedef int32_t		(*PFN_XMoveWindow) (Display *, Window, int32_t, int32_t);
typedef int32_t		(*PFN_XStoreName) (Display *, Window, const char *);
typedef int32_t		(*PFN_XChangeProperty) (Display *, Window, Atom, Atom, int32_t, int32_t, const unsigned char *, int32_t);
typedef char		*(*PFN_XGetAtomName) (Display *, Atom);
typedef Status		(*PFN_XGetAtomNames) (Display*, Atom *, int32_t, char **);
typedef char		*(*PFN_XGetDefault) (Display *, const char *, const char *);
typedef char		*(*PFN_XDisplayName) (const char *);

/* SECTION: Xutil.h
 * */
typedef Status		(*PFN_XAllocColor) (Display *, Colormap, XColor *);
typedef XWMHints	*(*PFN_XAllocWMHints) (void);
typedef Status		(*PFN_XChangeWindowAttributes) (Display *, Window, unsigned long, XSetWindowAttributes *);
typedef Status		(*PFN_XGetGeometry) (Display *, Drawable, Window *, int32_t *, int32_t *, uint32_t *, uint32_t *, uint32_t *, uint32_t *);
typedef Status		(*PFN_XGetWMHints) (Display *, Window, XWMHints *);
typedef Status		(*PFN_XGetWMNormalHints) (Display *, Window, XSizeHints *, long *);
typedef Status		(*PFN_XGetWMSizeHints) (Display *, Window, XSizeHints *, long *, Atom);
typedef Status		(*PFN_XGetIconSizes) (Display *, Window, XIconSize **, int32_t *);
typedef Status		(*PFN_XGetNormalHints) (Display *, Window, XSizeHints *);
typedef Status		(*PFN_XGetSizeHints) (Display *, Window, XSizeHints *, Atom);
typedef Status		(*PFN_XGetWMColormapWindows) (Display *, Window, Window **, int32_t *);
typedef int32_t		(*PFN_XGetWMProtocols) (Display *, Window, Atom **, int32_t *);
typedef Status		(*PFN_XGetZoomHints) (Display *, Window, XSizeHints *);
typedef int32_t		(*PFN_XIconifyWindow) (Display *, Window, int32_t);
typedef Status		(*PFN_XLookupColor) (Display *, Colormap, const char *, XColor *, XColor *);
typedef int32_t		(*PFN_XSetWMColormapWindows) (Display *, Window, Window *, int32_t);
typedef void		(*PFN_XSetWMHints) (Display *, Window, XWMHints *);
typedef Status		(*PFN_XSetWMIconName) (Display *, Window, XTextProperty *);
typedef void		(*PFN_XSetWMName) (Display *, Window, XTextProperty *);
typedef void		(*PFN_XSetWMNormalHints) (Display *, Window, XSizeHints *);
typedef Status		(*PFN_XSetWMProtocols) (Display *, Window, Atom *, int32_t);
typedef void		(*PFN_XSetWMSizeHints) (Display *, Window, XSizeHints *, Atom);
typedef Status		(*PFN_XWithdrawWindow) (Display *, Window, int32_t);
typedef Status		(*PFN_XMatchVisualInfo) (Display *, int32_t, int32_t, int32_t, XVisualInfo *);

/* SECTION: Xatom.h
 * */
typedef Atom		(*PFN_XInternAtom) (Display *, const char *, Bool);
typedef Status		(*PFN_XInternAtoms) (Display *, char **, int32_t, Bool, Atom *);

/* SECTION: XKBlib.h
 * */
typedef Bool		(*PFN_XkbQueryExtension) (Display *, int32_t *, int32_t *, int32_t *, int32_t *, int32_t *);
typedef KeySym		(*PFN_XkbKeycodeToKeysym) (Display *, KeyCode, int32_t, int32_t);
typedef Bool		(*PFN_XkbLibraryVersion) (int32_t *, int32_t *);
typedef Bool		(*PFN_XkbSetDetectableAutoRepeat) (Display *, Bool, Bool *);
typedef Bool		(*PFN_XkbGetKeyboard) (Display *, uint32_t, uint32_t);
typedef Bool		(*PFN_XkbGetMap) (Display *, uint32_t, uint32_t);

struct s_X11 {
	void	*handle;
	struct {
		/* SECTION: Xlib.h
		 * */
		PFN_XOpenDisplay					XOpenDisplay;
		PFN_XCloseDisplay					XCloseDisplay;
		PFN_XCreateColormap					XCreateColormap;
		PFN_XCreateWindow					XCreateWindow;
		PFN_XSelectInput					XSelectInput;
		PFN_XMapWindow						XMapWindow;
		PFN_XDestroyWindow					XDestroyWindow;
		PFN_XUnmapWindow					XUnmapWindow;
		PFN_XPending						XPending;
		PFN_XNextEvent						XNextEvent;
		PFN_XSendEvent						XSendEvent;
		PFN_XFlush							XFlush;
		PFN_XFree							XFree;
		PFN_XGetWindowProperty				XGetWindowProperty;
		PFN_XGetWindowAttributes			XGetWindowAttributes;
		PFN_XTranslateCoordinates			XTranslateCoordinates;
		PFN_XResizeWindow					XResizeWindow;
		PFN_XMoveWindow						XMoveWindow;
		PFN_XStoreName						XStoreName;
		PFN_XChangeProperty					XChangeProperty;
		PFN_XGetAtomName					XGetAtomName;
		PFN_XGetAtomNames					XGetAtomNames;
		PFN_XGetDefault						XGetDefault;
		PFN_XDisplayName					XDisplayName;

		/* SECTION: Xutil.h
		 * */
		PFN_XAllocColor						XAllocColor;
		PFN_XAllocWMHints					XAllocWMHints;
		PFN_XChangeWindowAttributes			XChangeWindowAttributes;
		PFN_XGetGeometry					XGetGeometry;
		PFN_XGetWMHints						XGetWMHints;
		PFN_XGetWMNormalHints				XGetWMNormalHints;
		PFN_XGetWMSizeHints					XGetWMSizeHints;
		PFN_XGetIconSizes					XGetIconSizes;
		PFN_XGetNormalHints					XGetNormalHints;
		PFN_XGetSizeHints					XGetSizeHints;
		PFN_XGetWMColormapWindows			XGetWMColormapWindows;
		PFN_XGetWMProtocols					XGetWMProtocols;
		PFN_XGetZoomHints					XGetZoomHints;
		PFN_XIconifyWindow					XIconifyWindow;
		PFN_XLookupColor					XLookupColor;
		PFN_XSetWMColormapWindows			XSetWMColormapWindows;
		PFN_XSetWMHints						XSetWMHints;
		PFN_XSetWMIconName					XSetWMIconName;
		PFN_XSetWMName						XSetWMName;
		PFN_XSetWMNormalHints				XSetWMNormalHints;
		PFN_XSetWMProtocols					XSetWMProtocols;
		PFN_XSetWMSizeHints					XSetWMSizeHints;
		PFN_XWithdrawWindow					XWithdrawWindow;
		PFN_XMatchVisualInfo				XMatchVisualInfo;

		/* SECTION: Xatom.h
		 * */
		PFN_XInternAtom						XInternAtom;
		PFN_XInternAtoms					XInternAtoms;

		/* SECTION: XKBlib.h
		 * */
        PFN_XkbQueryExtension               XkbQueryExtension;
        PFN_XkbKeycodeToKeysym              XkbKeycodeToKeysym;
        PFN_XkbLibraryVersion               XkbLibraryVersion;
        PFN_XkbSetDetectableAutoRepeat      XkbSetDetectableAutoRepeat;
        PFN_XkbGetKeyboard                  XkbGetKeyboard;
        PFN_XkbGetMap						XkbGetMap;
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
