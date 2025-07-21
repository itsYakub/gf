#include "./../gf.h"

#include <assert.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

/* Opaque pointer definition for X11 platform
 * */

struct s_window {
	
	struct {
		Display		*dsp;
		Window		id;
	} x11;
	
	struct {
		EGLDisplay	dsp;
		EGLConfig	conf;
		EGLSurface	surf;
		EGLContext	ctx;
	} egl;
		
	struct {
		Atom	wm_delete_window;
	} atoms;

	struct {
		t_event	lst[64];
		size_t	cnt;
	} events;

	struct {
		int32_t	id;
	} flags;

};

typedef struct s_window	*t_window;

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_connectDisplay(Display **);
GFAPIS bool	__gf_createWindow(Display *, Window *, const size_t, const size_t, const char *);
GFAPIS bool	__gf_createEGLContext(t_window);
GFAPIS bool	__gf_processAtoms(t_window);
GFAPIS bool	__gf_processFlags(t_window, const int32_t);



GFAPIS bool	__gf_destroyWindow(t_window);
GFAPIS bool	__gf_destroyContext(t_window);





/* SECTION:
 *  Static globals
 * */

static int32_t	g_egl_attr_conf[] = {
	EGL_RED_SIZE,			8,
	EGL_GREEN_SIZE,			8,
	EGL_BLUE_SIZE,			8,
	EGL_ALPHA_SIZE,			8,
	EGL_DEPTH_SIZE,			24,
	EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
	EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
	EGL_NONE
};
static int32_t	g_egl_attr_ctx[] = {
	EGL_CONTEXT_CLIENT_VERSION,	2,
	EGL_NONE
};





/* SECTION:
 *  X11 API implementation
 * */

GFAPI bool	gf_createWindow(t_window *win, const size_t w, const size_t h, const char *t, const int32_t f) {
	*win = (t_window) calloc(1, sizeof(struct s_window));

	assert(__gf_connectDisplay(&(*win)->x11.dsp));
	assert(__gf_createWindow((*win)->x11.dsp, &(*win)->x11.id, w, h, t));
	assert(__gf_createEGLContext(*win));
	
	assert(__gf_processAtoms(*win));
	assert(__gf_processFlags(*win, f));
	return (true);
}

GFAPIS	bool	__gf_connectDisplay(Display **dptr) {
	*dptr = XOpenDisplay(0);
	if (!*dptr) {
		gf_loge("DISPLAY: Failed to create\n");
		return (false);
	}
	gf_logi("DISPLAY: Created successfully\n");
	return (true);
}


GFAPIS	bool	__gf_createWindow(Display *dsp, Window *wptr, const size_t w, const size_t h, const char * t) {
	XSetWindowAttributes	_attr;
	XWMHints				_hints;

	memset(&_attr, 0, sizeof(XSetWindowAttributes));
	_attr.event_mask =
		KeyPressMask | KeyReleaseMask |
		ButtonPressMask | ButtonReleaseMask |
		Button1Mask | Button2Mask | Button3Mask |
		PointerMotionMask |
		ExposureMask | StructureNotifyMask | ResizeRedirectMask |
		ClientMessage |
		CWColormap | CWBorderPixel | CWBackPixel | CWEventMask;

	*wptr = XCreateWindow(
		dsp, DefaultRootWindow(dsp),
		0, 0, w, h, 0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWEventMask,
		&_attr
	);
	gf_logi("WINDOW: Created successfully\n");
	/* If title isn't null, then store it in the window's name
	 * */
	if (t) {
		XStoreName(dsp, *wptr, t);
		gf_logi("WINDOW: Title set: %s\n", t);
	}
	
	XSelectInput(dsp, *wptr, 0);

	memset(&_hints, 0, sizeof(XWMHints));
	_hints.input = true;
	_hints.flags = InputHint;
	XSetWMHints(dsp, *wptr, &_hints);

	/* Map the window onto the root window
	 * */
	XMapWindow(dsp, *wptr);
	gf_logi("WINDOW: Mapped successfully\n");
	return (true);
}

GFAPIS	bool	__gf_createEGLContext(t_window win) {
	int32_t	_attr_cont_cnt;

	/* Create EGL display object based on X11 object
	 * */
	win->egl.dsp = eglGetDisplay((EGLNativeDisplayType) win->x11.dsp);
	if (!win->egl.dsp) {
		gf_loge("EGL: Display failed\n");
		return (false);
	}
	if (!eglInitialize(win->egl.dsp, 0, 0)) {
		gf_loge("EGL: Initialization failed\n");
		return (false);
	}

	/* Create config object
	 * */
	_attr_cont_cnt = 0;
	if (!eglChooseConfig(win->egl.dsp, g_egl_attr_conf, &win->egl.conf, 1, &_attr_cont_cnt)) {
		gf_loge("EGL: Config failed\n");
		return (false);
	}

	/* Create EGL surface based on X11 window
	 * */
	win->egl.surf = eglCreateWindowSurface(win->egl.dsp, win->egl.conf, (EGLNativeWindowType) win->x11.id, 0);
	if (!win->egl.surf) {
		gf_loge("EGL: Surface failed\n");
		return (false);
	}

	/* Create EGL Context
	 * */
	win->egl.ctx = eglCreateContext(win->egl.dsp, win->egl.conf, EGL_NO_CONTEXT, g_egl_attr_ctx);
	if (!win->egl.ctx) {
		gf_loge("EGL: Context failed\n");
		return (false);
	}

	/* Make the EGL context current
	 * */
	if (!eglMakeCurrent(win->egl.dsp, win->egl.surf, win->egl.surf, win->egl.ctx)) {
		gf_loge("EGL: Context current failed\n");
		return (false);
	}

	if (!eglBindAPI(EGL_OPENGL_API)) {
		gf_loge("EGL: Failed to bind OpenGL api\n");
		return (false);
	}

	gf_logi("EGL: Created successfully\n");
	gf_logi("EGL: Client: %s\n", eglQueryString(win->egl.dsp, EGL_CLIENT_APIS));
	gf_logi("EGL: Version: %s\n", eglQueryString(win->egl.dsp, EGL_VERSION));
	gf_logi("EGL: Vendor: %s\n", eglQueryString(win->egl.dsp, EGL_VENDOR));

	return (true);
}

GFAPIS bool	__gf_processAtoms(t_window win) {
	win->atoms.wm_delete_window = XInternAtom(win->x11.dsp, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(win->x11.dsp, win->x11.id, &win->atoms.wm_delete_window, 1);
	return (true);
}

GFAPIS bool	__gf_processFlags(t_window win, const int32_t f) {
	/* First, let's set the initial window flags.
	 * Then we can process them!
	 * */
	win->flags.id = f;

	/* Window flag: GF_WINDOW_RESIZABLE
	 * NOTE(yakub):
	 *  This flag can be ignored by the WM. I can tell that on Mutter it works fine.
	 *  For the testing purposes I should test in on other WMs, like i3.
	 * */
	if ((win->flags.id & GF_WINDOW_RESIZABLE)) {
		/* NOTE(yakub):
		 *  X11 enables window resizing by default. Our job here is to disable it if there's no 'GF_WINDOW_RESIZABLE' flag.
		 *  We can keep this flag for a simple logging tho...
		 * */
		gf_logi("WINDOW: Flag: RESIZABLE\n");
	}
	else {
		XSizeHints	_hints;
		int64_t		_toto;
		int32_t		_width;
		int32_t		_height;

		gf_getWindowSize(win, &_width, &_height);
		
		XGetWMNormalHints(win->x11.dsp, win->x11.id, &_hints, &_toto);
		
		_hints.width = _width;
		_hints.min_width = _width;
		_hints.max_width = _width;
		_hints.height = _height;
		_hints.min_height = _height;
		_hints.max_height = _height;
		_hints.flags = PPosition | PSize | PMinSize | PMaxSize;

		XSetWMNormalHints(win->x11.dsp, win->x11.id, &_hints);
	}

	/* Window flag: GF_WINDOW_VSYNC_HINT
	 * */
	if ((win->flags.id & GF_WINDOW_VSYNC_HINT)) {
		if (!eglSwapInterval(win->egl.dsp, 1)) {
			gf_loge("EGL: Failed to set a swap interval\n");
		}
		gf_logi("WINDOW: Flag: VSYNC_HINT\n");
	}
	return (true);
}





GFAPI bool	gf_destroyWindow(t_window win) {
	assert(__gf_destroyContext(win));
	assert(__gf_destroyWindow(win));
	free(win);
	return (true);
}

GFAPIS	bool	__gf_destroyWindow(t_window win) {
	XDestroyWindow(win->x11.dsp, win->x11.id);
	gf_logi("WINDOW: Destroyed successfully\n");
	XCloseDisplay(win->x11.dsp);
	gf_logi("DISPLAY: Destroyed successfully\n");
	
	return (true);
}

GFAPIS	bool	__gf_destroyContext(t_window win) {
	eglMakeCurrent(win->egl.dsp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(win->egl.dsp, win->egl.ctx);
	eglDestroySurface(win->egl.dsp, win->egl.surf);
	eglTerminate(win->egl.dsp);
	gf_logi("EGL: Terminated successfully\n");
	return (true);
}





GFAPI bool	gf_pollEvents(t_window win, t_event *event) {
	XEvent	_x11_event;
	t_event	_gf_event;

	/* Poll gf events from event queue
	 * */
	while ((int) win->events.cnt > 0) {
		*event = win->events.lst[win->events.cnt - 1];
		gf_popEvent(win, event);
		return (true);
	}

	/* Event queue emtpy, now we need to poll the events from implementation
	 * */
	while (XPending(win->x11.dsp)) {
		memset(&_gf_event, 0, sizeof(t_event));
		XNextEvent(win->x11.dsp, &_x11_event);
		switch (_x11_event.type) {
			case (ClientMessage): {
				if ((Atom) _x11_event.xclient.data.l[0] == win->atoms.wm_delete_window) {
					_gf_event.type = GF_EVENT_QUIT;
					gf_pushEvent(win, &_gf_event);
				}
			} break;
			
		}

	}
	/* As there was no event to be retrieved, we return the 'none' event
	 * */
	memset(event, 0, sizeof(t_event));
	return (false);
}

GFAPI bool	gf_popEvent(t_window win, t_event *e) {
	(void) e;
	if (win->events.cnt <= 0) {
		return (false);
	}
	memset(&win->events.lst[--win->events.cnt], 0, sizeof(t_event));
	return (true);
}

GFAPI bool	gf_pushEvent(t_window win, t_event *e) {
	memcpy(&win->events.lst[win->events.cnt++], e, sizeof(t_event));
	return (true);
}





GFAPI bool	gf_swapBuffers(t_window win) {
	if (!win) {
		return (false);
	}

	eglSwapBuffers(win->egl.dsp, win->egl.surf);
	return (true);
}



GFAPI bool	gf_getWindowSize(t_window win, int32_t *wptr, int32_t *hptr) {
	XWindowAttributes	_attr;

	memset(&_attr, 0, sizeof(XWindowAttributes));
	if (!XGetWindowAttributes(win->x11.dsp, win->x11.id, &_attr)) {
		return (false);
	}
	*wptr = _attr.width;
	*hptr = _attr.height;
	return (true);
}
