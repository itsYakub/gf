#include "./../gf.h"
#include "./gf-int.h"

#include <assert.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* SECTION:
 *  Static globals
 * */

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

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_connectDisplay(t_window);
GFAPIS bool	__gf_createVisualInfo(t_window);
GFAPIS bool	__gf_createWindow(t_window, const size_t, const size_t, const char *);
GFAPIS bool	__gf_processAtoms(t_window);

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createWindow(t_window *win, const size_t w, const size_t h, const char *t) {
	*win = (t_window) calloc(1, sizeof(struct s_window));

	assert(__gf_connectDisplay(*win));

#if defined (USE_GLX)
	assert(__gf_createVisualInfo(*win));
#endif

	assert(__gf_createWindow(*win, w, h, t));
	
	assert(gf_getWindowSize(*win, 0, 0));
	assert(gf_getWindowPosition(*win, 0, 0));
	
	assert(__gf_processAtoms(*win));

	/* Setting up default window settings
	 * */	
	gf_setWindowResizable(*win, false);
	
	return (true);
}

/* SECTION:
 *  Private interface definitions
 * */

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
