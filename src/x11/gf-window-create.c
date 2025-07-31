#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Static globals
 * */

#if defined (USE_GLX)
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
#endif

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

	/* Setting up default window configs
	 * */	
	gf_setWindowResizable(*win, false);
	gf_setWindowBorderless(*win, false);
	gf_setWindowFullscreen(*win, false);

	return (true);
}

/* SECTION:
 *  Private interface definitions
 * */

GFAPIS	bool	__gf_connectDisplay(t_window win) {
	win->x11.dsp = XOpenDisplay(0);
	if (!win->x11.dsp) {

#if defined (VERBOSE)
		gf_loge("DISPLAY: Failed to connect\n");
#endif

		return (false);
	}

#if defined (VERBOSE)
	gf_logi("DISPLAY: Connected successfully\n");
#endif

	win->x11.root_id = DefaultRootWindow(win->x11.dsp);
	if (!win->x11.root_id) {

#if defined (VERBOSE)
		gf_loge("ROOT: Inaccessible\n");
#endif

		return (false);
	}

#if defined (VERBOSE)
	gf_logi("ROOT: Accessed | ID.:%li\n", win->x11.root_id);
#endif

	win->x11.screen_id = DefaultScreen(win->x11.dsp);

#if defined (VERBOSE)
	gf_logi("SCREEN: Accessed | ID.:%d\n", win->x11.screen_id);
#endif


	return (true);
}

GFAPIS bool	__gf_createVisualInfo(t_window win) {

#if defined (USE_GLX)
	GLXFBConfig	*_fbconf_arr;
	int32_t		_fbconf_best,
				_fbconf_count,
				_fbconf_samples,
				_fbconf_sample_buffers;

	_fbconf_best = -1;
	_fbconf_arr = glXChooseFBConfig(
		win->x11.dsp,
		win->x11.screen_id,
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
#endif

	(void) win;
	return (false);
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
		ExposureMask | StructureNotifyMask | PropertyChangeMask | 
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
		XMatchVisualInfo(win->x11.dsp, win->x11.screen_id, 24, TrueColor, win->x11.info);
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
	_attr.colormap = XCreateColormap(win->x11.dsp, win->x11.root_id, win->x11.info->visual, AllocNone);
	_attr.background_pixmap = None;
	_attr.background_pixel = 0;
	_attr.border_pixel = 0;
	_attr.event_mask = _event_mask;

	/* Creating the window itself
	 * */
	win->x11.id = XCreateWindow(
		win->x11.dsp, win->x11.root_id,
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
	win->atoms.wm_delete_window = XInternAtom(win->x11.dsp, "WM_DELETE_WINDOW", false);
	win->atoms.wm_net_state = XInternAtom(win->x11.dsp, "_NET_WM_STATE", false);
	win->atoms.wm_net_state_above = XInternAtom(win->x11.dsp, "_NET_WM_STATE_ABOVE", false);
	win->atoms.wm_net_state_fullscreen = XInternAtom(win->x11.dsp, "_NET_WM_STATE_FULLSCREEN", false);
	win->atoms.wm_net_state_hidden = XInternAtom(win->x11.dsp, "_NET_WM_STATE_HIDDEN", false);
	win->atoms.wm_net_state_maximized_horz = XInternAtom(win->x11.dsp, "_NET_WM_STATE_MAXIMIZED_HORZ", false);
	win->atoms.wm_net_state_maximized_vert = XInternAtom(win->x11.dsp, "_NET_WM_STATE_MAXIMIZED_VERT", false);
	win->atoms.wm_net_window_type = XInternAtom(win->x11.dsp, "_NET_WM_WINDOW_TYPE", false);
	win->atoms.wm_net_window_normal = XInternAtom(win->x11.dsp, "_NET_WM_WINDOW_NORMAL", false);
	win->atoms.wm_net_window_dock = XInternAtom(win->x11.dsp, "_NET_WM_WINDOW_DOCK", false);
	XSetWMProtocols(win->x11.dsp, win->x11.id, &win->atoms.wm_delete_window, 1);
	return (true);
}
