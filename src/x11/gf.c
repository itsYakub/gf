#include "./../gf.h"

#include <errno.h>
#include <dlfcn.h>
#include <assert.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GL/glx.h>
#include <GL/glxext.h>

#include <GL/gl.h>

/* Opaque pointer definition for X11 platform
 * */

struct s_window {
	
	struct {
		Display		*dsp;
		XVisualInfo	*info;
		Window		id;
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
	} atoms;

	struct {
		t_event	lst[64];
		size_t	cnt;
	} events;

	struct {
		int32_t	id;
	} flags;

	struct {
		int32_t	width;
		int32_t	height;
		int32_t	x;
		int32_t	y;
	} misc;
};

typedef struct s_window	*t_window;

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_connectDisplay(t_window);
GFAPIS bool	__gf_createVisualInfo(t_window);
GFAPIS bool	__gf_createWindow(t_window, const size_t, const size_t, const char *);

GFAPIS bool	__gf_createContext(t_window);
GFAPIS bool	__gf_createContextEGL(t_window);
GFAPIS bool	__gf_createContextGLX(t_window);
GFAPIS bool	__gf_loadGLXExtensions(void);

GFAPIS bool	__gf_processAtoms(t_window);
GFAPIS bool	__gf_processFlags(t_window);



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

static int32_t	g_glx_attr_conf[] = {
	GLX_USE_GL,				1,
	GLX_DOUBLEBUFFER,		1,
	GLX_DEPTH_SIZE,         24,                            
	GLX_RED_SIZE,           8,
	GLX_GREEN_SIZE,         8,
	GLX_BLUE_SIZE,          8,
	GLX_ALPHA_SIZE,         8,
	GLX_X_VISUAL_TYPE,      GLX_TRUE_COLOR,
	GLX_RENDER_TYPE,        GLX_RGBA_BIT,
	GLX_DRAWABLE_TYPE,      GLX_WINDOW_BIT,
	None
};

static int32_t	g_glx_attr_ctx[] = {
	GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
	GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
	GLX_CONTEXT_MINOR_VERSION_ARB, 3,
	None
};

static PFNGLXCREATECONTEXTATTRIBSARBPROC	glXCreateContextAttribsARB;
static PFNGLXSWAPINTERVALEXTPROC			glXSwapIntervalEXT;





/* SECTION:
 *  X11 API implementation
 * */

GFAPI bool	gf_createWindow(t_window *win, const size_t w, const size_t h, const char *t) {
	*win = (t_window) calloc(1, sizeof(struct s_window));

	assert(__gf_connectDisplay(*win));
	assert(__gf_createVisualInfo(*win));
	assert(__gf_createWindow(*win, w, h, t));
	
	assert(gf_getWindowSize(*win, 0, 0));
	assert(gf_getWindowPosition(*win, 0, 0));
	
	assert(__gf_processAtoms(*win));
	assert(__gf_processFlags(*win));
	return (true);
}

GFAPIS	bool	__gf_connectDisplay(t_window win) {
	win->x11.dsp = XOpenDisplay(0);
	if (!win->x11.dsp) {

#if defined (VERBOSE)
		gf_loge("DISPLAY: Failed to create\n");
#endif

		return (false);
	}

#if defined (VERBOSE)
	gf_logi("DISPLAY: Created successfully\n");
#endif

	return (true);
}

GFAPIS bool	__gf_createVisualInfo(t_window win) {
	GLXFBConfig	*_fbconf_arr;
	int32_t		_fbconf_best,
				_fbconf_count,
				_fbconf_samples,
				_fbconf_sample_buffers;

	_fbconf_best = -1;
	_fbconf_arr = glXChooseFBConfig(
		win->x11.dsp,
		DefaultScreen(win->x11.dsp),
		g_glx_attr_conf,
		&_fbconf_count
	);
	for (size_t i = 0; i < (size_t) _fbconf_count; i++) {
		win->x11.info = glXGetVisualFromFBConfig(win->x11.dsp, _fbconf_arr[i]);
		if (!win->x11.info) {
			continue;
		}
		free(win->x11.info);
		glXGetFBConfigAttrib(win->x11.dsp, _fbconf_arr[i], GLX_SAMPLE_BUFFERS, &_fbconf_sample_buffers);
		glXGetFBConfigAttrib(win->x11.dsp, _fbconf_arr[i], GLX_SAMPLES, &_fbconf_samples);
		if ((_fbconf_best < 0 || _fbconf_sample_buffers) && (!_fbconf_samples && _fbconf_best == -1)) {
			_fbconf_best = i;
		}
	}
	if (_fbconf_best == -1) {
	
#if defined (VERBOSE)
		gf_loge("GLX: GLXFBConfig failed\n");
#endif
	
		return (false);
	}
	win->glx.conf = _fbconf_arr[_fbconf_best];
	free(_fbconf_arr);
	win->x11.info = glXGetVisualFromFBConfig(win->x11.dsp, win->glx.conf);
	if (!win->x11.info) {
			
#if defined (VERBOSE)
		gf_loge("GLX: XVisualInfo failed\n");
#endif
		
		return (false);
	}
	return (true);
}

GFAPIS	bool	__gf_createWindow(t_window win, const size_t w, const size_t h, const char * t) {
	XSetWindowAttributes	_attr;
	XWMHints				_hints;
	int32_t					_event_mask;

	/* Set the value of an event mask
	 * */
	_event_mask =
		KeyPressMask | KeyReleaseMask |
		ButtonPressMask | ButtonReleaseMask |
		Button1Mask | Button2Mask | Button3Mask |
		PointerMotionMask |
		ExposureMask | StructureNotifyMask | 
		ClientMessage;

	/* If XVisualInfo is missing, allocate it manually
	 * */
	if (!win->x11.info) {
		win->x11.info = malloc(sizeof(XVisualInfo));
		if (!win->x11.info) {

#if defined (VERBOSE)
				gf_loge("VISUAL: Allocation failed\n");
#endif

			return (false);
		}
		XMatchVisualInfo(win->x11.dsp, DefaultScreen(win->x11.dsp), 24, TrueColor, win->x11.info);
		if (!win->x11.info) {

#if defined (VERBOSE)
				gf_loge("VISUAL: Match failed\n");
#endif

			return (false);
		}
	}

	/* Creating window attributes
	 * */
	memset(&_attr, 0, sizeof(XSetWindowAttributes));
	_attr.colormap = XCreateColormap(win->x11.dsp, DefaultRootWindow(win->x11.dsp), win->x11.info->visual, AllocNone);
	_attr.background_pixmap = None;
	_attr.background_pixel = 0;
	_attr.border_pixel = 0;
	_attr.event_mask = _event_mask;

	/* Creating the window itself
	 * */
	win->x11.id = XCreateWindow(
		win->x11.dsp, DefaultRootWindow(win->x11.dsp),
		0, 0, w, h, 0,
		win->x11.info->depth,
		InputOutput,
		win->x11.info->visual,
		CWColormap | CWBorderPixel | CWBackPixel | CWEventMask,
		&_attr
	);

#if defined (VERBOSE)
	gf_logi("WINDOW: Created successfully\n");
#endif

	/* If title isn't null, then store it in the window's name
	 * */
	if (t) {
		XStoreName(win->x11.dsp, win->x11.id, t);

#if defined (VERBOSE)
		gf_logi("WINDOW: Title set: %s\n", t);
#endif

	}
	
	memset(&_hints, 0, sizeof(XWMHints));
	_hints.input = true;
	_hints.flags = InputHint;
	XSetWMHints(win->x11.dsp, win->x11.id, &_hints);
	XSelectInput(win->x11.dsp, win->x11.id, _event_mask);
	XMapWindow(win->x11.dsp, win->x11.id);

#if defined (VERBOSE)
	gf_logi("WINDOW: Mapped successfully\n");
#endif

	return (true);
}

GFAPIS bool	__gf_processAtoms(t_window win) {
	win->atoms.wm_delete_window = XInternAtom(win->x11.dsp, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(win->x11.dsp, win->x11.id, &win->atoms.wm_delete_window, 1);
	return (true);
}

GFAPIS bool	__gf_processFlags(t_window win) {
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

#if defined (VERBOSE)
		gf_logi("WINDOW: Flag: RESIZABLE\n");
#endif

	}
	else {
		XSizeHints	_hints;
		int64_t		_toto;
		int32_t		_width;
		int32_t		_height;

		_width = _height = 0;
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
		if (win->egl.ctx) {
			if (!eglSwapInterval(win->egl.dsp, 1)) {

#if defined (VERBOSE)
				gf_loge("EGL: Failed to set a swap interval\n");
#endif

			}
		}
		else if (win->glx.ctx) {
			glXSwapIntervalEXT(win->x11.dsp, win->x11.id, 1);
		}

#if defined (VERBOSE)
		gf_logi("WINDOW: Flag: VSYNC_HINT\n");
#endif

	}
	return (true);
}

GFAPI bool	gf_makeCurrent(t_window win) {
	if (!win->glx.ctx && !win->egl.ctx) {
		__gf_createContext(win);
	}

#if defined (USE_GLX)
	if (!glXMakeCurrent(win->x11.dsp, win->x11.id, win->glx.ctx)) {

# if defined (VERBOSE)
		gf_loge("GLX: Context current failed\n");
# endif

		return (false);
	}
#elif defined (USE_EGL)
	if (!eglMakeCurrent(win->egl.dsp, win->egl.surf, win->egl.surf, win->egl.ctx)) {

# if defined (VERBOSE)
		gf_loge("EGL: Context current failed\n");
# endif

		return (false);
	}
#endif

#if defined (VERBOSE)
		gf_logi("GL: Version: %s\n", glGetString(GL_VERSION));
		gf_logi("GL: Vendor: %s\n", glGetString(GL_VENDOR));
		gf_logi("GL: Renderer: %s\n", glGetString(GL_RENDERER));
		
		gf_logi("GLSL: Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

		return (true);
}

GFAPIS bool	__gf_createContext(t_window win) {

#if defined (USE_GLX)
	return (__gf_createContextGLX(win));
#elif defined (USE_EGL)
	return (__gf_createContextEGL(win));
#else
	return (false);
#endif

}

GFAPIS bool	__gf_createContextEGL(t_window win) {
	int32_t	_attr_cont_cnt;

	/* Create EGL display object based on X11 object
	 * */
	win->egl.dsp = eglGetDisplay((EGLNativeDisplayType) win->x11.dsp);
	if (!win->egl.dsp) {

#if defined (VERBOSE)
		gf_loge("EGL: Display failed\n");
#endif

		return (false);
	}
	if (!eglInitialize(win->egl.dsp, 0, 0)) {

#if defined (VERBOSE)
		gf_loge("EGL: Initialization failed\n");
#endif

		return (false);
	}

	/* Create config object
	 * */
	_attr_cont_cnt = 0;
	if (!eglChooseConfig(win->egl.dsp, g_egl_attr_conf, &win->egl.conf, 1, &_attr_cont_cnt)) {

#if defined (VERBOSE)
		gf_loge("EGL: Config failed\n");
#endif

		return (false);
	}

	/* Create EGL surface based on X11 window
	 * */
	win->egl.surf = eglCreateWindowSurface(win->egl.dsp, win->egl.conf, (EGLNativeWindowType) win->x11.id, 0);
	if (!win->egl.surf) {

#if defined (VERBOSE)
		gf_loge("EGL: Surface failed\n");
#endif

		return (false);
	}

	/* Create EGL Context
	 * */
	win->egl.ctx = eglCreateContext(win->egl.dsp, win->egl.conf, EGL_NO_CONTEXT, g_egl_attr_ctx);
	if (!win->egl.ctx) {

#if defined (VERBOSE)
		gf_loge("EGL: Context failed\n");
#endif

		return (false);
	}

	if (!eglBindAPI(EGL_OPENGL_API)) {

#if defined (VERBOSE)
		gf_loge("EGL: Failed to bind OpenGL api\n");
#endif

		return (false);
	}

#if defined (VERBOSE)
	gf_logi("EGL: Created successfully\n");
	gf_logi("EGL: Client: %s\n", eglQueryString(win->egl.dsp, EGL_CLIENT_APIS));
	gf_logi("EGL: Version: %s\n", eglQueryString(win->egl.dsp, EGL_VERSION));
	gf_logi("EGL: Vendor: %s\n", eglQueryString(win->egl.dsp, EGL_VENDOR));
#endif

	return (true);
}

GFAPIS bool	__gf_createContextGLX(t_window win) {
	if (!__gf_loadGLXExtensions()) {

#if defined (VERBOSE)
		gf_loge("GLX: Failed to load extensions\n");
#endif

		return (false);
	}

	if (glXCreateContextAttribsARB) {
		win->glx.ctx = glXCreateContextAttribsARB(win->x11.dsp, win->glx.conf, 0, 1, g_glx_attr_ctx);
		if (!win->glx.ctx) {
			return (false);
		}
	}
	else {
		win->glx.ctx = glXCreateContext(win->x11.dsp, win->x11.info, 0, 1);
		if (!win->glx.ctx) {
			return (false);
		}
	}

#if defined (VERBOSE)
	gf_logi("GLX: Created successfully\n");
	gf_logi("GLX: Version: %s\n", glXGetClientString(win->x11.dsp, GLX_VERSION));
	gf_logi("GLX: Vendor: %s\n", glXGetClientString(win->x11.dsp, GLX_VENDOR));
#endif

	return (true);
}

GFAPIS bool	__gf_loadGLXExtensions(void) {
	glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((GLubyte *) "glXCreateContextAttribsARB");
	if (!glXCreateContextAttribsARB) {
		return (false);
	}
	glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((GLubyte *) "glXSwapIntervalEXT");
	if (!glXSwapIntervalEXT) {
		return (false);
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

#if defined (VERBOSE)
	gf_logi("WINDOW: Destroyed successfully\n");
#endif

	free(win->x11.info);

#if defined (VERBOSE)
	gf_logi("VISUAL: Freed successfully\n");
#endif

	XCloseDisplay(win->x11.dsp);

#if defined (VERBOSE)
	gf_logi("DISPLAY: Destroyed successfully\n");
#endif
	return (true);
}

GFAPIS	bool	__gf_destroyContext(t_window win) {
	if (!win->glx.ctx) {
		eglMakeCurrent(win->egl.dsp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(win->egl.dsp, win->egl.ctx);
		eglDestroySurface(win->egl.dsp, win->egl.surf);
		eglTerminate(win->egl.dsp);

#if defined (VERBOSE)
		gf_logi("EGL: Terminated successfully\n");
#endif

		return (true);
	}
	else if (!win->egl.ctx) {
		glXDestroyContext(win->x11.dsp, win->glx.ctx);

#if defined (VERBOSE)
		gf_logi("GLX: Terminated successfully\n");
#endif

		return (true);
	}
	return (false);
}





GFAPI bool	gf_pollEvents(t_window win, t_event *event) {
	XEvent	_x11_event;
	t_event	_gf_event;

	/* Poll gf events from event queue
	 * */
	{
		while ((int) win->events.cnt > 0) {
			*event = win->events.lst[win->events.cnt - 1];
			gf_popEvent(win, event);
			return (true);
		}
	}

	/* Event queue emtpy, now we need to poll the events from implementation
	 * */
	{
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
				
				case (ConfigureNotify): {
					if (
						win->misc.width != _x11_event.xconfigure.width ||
						win->misc.height != _x11_event.xconfigure.height
					) {
						_gf_event.type = GF_EVENT_RESIZE;
						_gf_event.s_data.i[0] = win->misc.width = _x11_event.xconfigure.width;
						_gf_event.s_data.i[1] = win->misc.height = _x11_event.xconfigure.height;
					}
					else if (
						win->misc.x != _x11_event.xconfigure.x ||
						win->misc.y != _x11_event.xconfigure.y
					) {
						_gf_event.type = GF_EVENT_MOVE;
						_gf_event.s_data.i[0] = win->misc.x = _x11_event.xconfigure.x;
						_gf_event.s_data.i[1] = win->misc.y = _x11_event.xconfigure.y;
					}
					else { break; }
					gf_pushEvent(win, &_gf_event);
				} break;	

				case (MotionNotify): {
					_gf_event.type = GF_EVENT_MOUSEMOTION;
					_gf_event.s_data.i[0] = _x11_event.xmotion.x;
					_gf_event.s_data.i[1] = _x11_event.xmotion.y;
					_gf_event.s_data.i[2] = _x11_event.xmotion.x_root;
					_gf_event.s_data.i[3] = _x11_event.xmotion.y_root;
					gf_pushEvent(win, &_gf_event);
				} break;
				/* NOTE(yakub):
				 *  Under the hood, x11 maps the mouse buttons to:
				 *  - [1] -> LEFT;
				 *  - [2] -> MIDDLE;
				 *  - [3] -> RIGHT;
				 * */
				case (ButtonPress): {
					_gf_event.type = GF_EVENT_MOUSE_PRESS;
					if (_x11_event.xbutton.button == 1) { _gf_event.s_data.c[0] = GF_BUTTON_LEFT; }
					else if (_x11_event.xbutton.button == 2) { _gf_event.s_data.c[0] = GF_BUTTON_MIDDLE; }
					else if (_x11_event.xbutton.button == 3) { _gf_event.s_data.c[0] = GF_BUTTON_RIGHT; }
					else { break; }
					_gf_event.s_data.c[1] = true;
					gf_pushEvent(win, &_gf_event);
				} break;
				case (ButtonRelease): {
					_gf_event.type = GF_EVENT_MOUSE_RELEASE;
					if (_x11_event.xbutton.button == 1) { _gf_event.s_data.c[0] = GF_BUTTON_LEFT; }
					else if (_x11_event.xbutton.button == 2) { _gf_event.s_data.c[0] = GF_BUTTON_MIDDLE; }
					else if (_x11_event.xbutton.button == 3) { _gf_event.s_data.c[0] = GF_BUTTON_RIGHT; }
					else { break; }
					_gf_event.s_data.c[1] = false;
					gf_pushEvent(win, &_gf_event);
				} break;
			}

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

	if (win->egl.ctx) {
		eglSwapBuffers(win->egl.dsp, win->egl.surf);
		return (true);
	}
	else if(win->glx.ctx) {
		glXSwapBuffers(win->x11.dsp, win->x11.id);
		return (true);
	}
	return (false);
}



GFAPI bool	gf_getWindowSize(t_window win, int32_t *wptr, int32_t *hptr) {
	XWindowAttributes	_attr;

	memset(&_attr, 0, sizeof(XWindowAttributes));
	if (!XGetWindowAttributes(win->x11.dsp, win->x11.id, &_attr)) {
		return (false);
	}
	
	/* Updating internal variables
	 * */
	win->misc.width = _attr.width;
	win->misc.height = _attr.height;

	/* Setting the value of the 'wptr' and 'hptr' (memory safe)
	 * */
	if (wptr) {
		*wptr = _attr.width;
	}
	if (hptr) {
		*hptr = _attr.height;
	}
	return (true);
}

GFAPI bool	gf_getWindowPosition(t_window win, int32_t *xptr, int32_t *yptr) {
	XWindowAttributes	_attr;
	Window				_child;
	int32_t				_x, _y;

	memset(&_attr, 0, sizeof(XWindowAttributes));
	if (!XGetWindowAttributes(win->x11.dsp, win->x11.id, &_attr)) {
		return (false);
	}

	_x = _y = 0;
	XTranslateCoordinates(
		win->x11.dsp, win->x11.id, DefaultRootWindow(win->x11.dsp),
		0, 0, &_x, &_y,
		&_child
	);
	
	/* Updating internal variables
	 * */
	win->misc.x = _x - _attr.x;
	win->misc.y = _y - _attr.y;

	/* Setting the value of the 'wptr' and 'hptr' (memory safe)
	 * */
	if (xptr) {
		*xptr = _attr.x;
	}
	if (yptr) {
		*yptr = _attr.y;
	}
	return (true);
}

GFAPI void	*gf_getProcAddress(const char *name) {
	void	*_handle;
	void	*_ptr;

	_handle = dlopen("libGL.so.1", RTLD_LAZY);
	if (!_handle) {

#if defined (VERBOSE)
		gf_logi("GF: %s\n", strerror(errno));
#endif

		return (0);
	}
	_ptr = dlsym(_handle, name);
	if (!_ptr) {

#if defined (VERBOSE)
		gf_logi("GF: %s\n", strerror(errno));
#endif

		return (0);
	}
	return (_ptr);
}
