#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Static globals
 * */

#if defined (USE_GLX)
static int32_t	g_context_attr_conf[] = {
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
GFAPIS bool	__gf_createWindow(t_window, const size_t, const size_t);
GFAPIS bool	__gf_processAtoms(t_window);





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createWindow(t_window *win, const size_t w, const size_t h, const char *t) {
	t_window	_win;

	_win = (t_window) calloc(1, sizeof(struct s_window));

	assert(__gf_connectDisplay(_win));
	assert(__gf_createVisualInfo(_win));
	assert(__gf_createWindow(_win, w, h));
	assert(__gf_processAtoms(_win));

	gf_setWindowSize(_win, w, h);
	gf_setWindowTitle(_win, t);

	gf_int_updateWindowConfig(_win);
	
	*win = _win;
	return (true);
}

GFAPI bool	gf_destroyWindow(t_window win) {
	if (win->client.id) XDestroyWindow(win->client.dsp, win->client.id);
	if (win->client.info) free(win->client.info);
	if (win->client.dsp) XCloseDisplay(win->client.dsp);

	free(win);
	return (true);
}





/* SECTION:
 *  Private interface definitions
 * */

GFAPIS	bool	__gf_connectDisplay(t_window win) {
	win->client.dsp = XOpenDisplay(0);
	if (!win->client.dsp) {

#if defined (VERBOSE)
		gf_loge("WINDOW: Failed to connect\n");
#endif

		return (false);
	}
	
	win->client.root_id = DefaultRootWindow(win->client.dsp);
	win->client.screen_id = DefaultScreen(win->client.dsp);
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
		win->client.dsp,
		win->client.screen_id,
		g_context_attr_conf,
		&_fbconf_count
	);
	for (size_t i = 0; i < (size_t) _fbconf_count; i++) {
		win->client.info = glXGetVisualFromFBConfig(win->client.dsp, _fbconf_arr[i]);
		if (!win->client.info) {
			continue;
		}
		free(win->client.info);
		glXGetFBConfigAttrib(win->client.dsp, _fbconf_arr[i], GLX_SAMPLE_BUFFERS, &_fbconf_sample_buffers);
		glXGetFBConfigAttrib(win->client.dsp, _fbconf_arr[i], GLX_SAMPLES, &_fbconf_samples);
		if ((_fbconf_best < 0 || _fbconf_sample_buffers) && (!_fbconf_samples && _fbconf_best == -1)) {
			_fbconf_best = i;
		}
	}
	if (_fbconf_best == -1) {
	
#if defined (VERBOSE)
		gf_loge("CONTEXT: Configuration failed\n");
#endif
	
		return (false);
	}
	win->context.conf = _fbconf_arr[_fbconf_best];
	free(_fbconf_arr);
	win->client.info = glXGetVisualFromFBConfig(win->client.dsp, win->context.conf);
	if (!win->client.info) {
			
#if defined (VERBOSE)
		gf_loge("CONTEXT: VisualInfo failed\n");
#endif
		
		return (false);
	}
	return (true);

#else

	win->client.info = malloc(sizeof(XVisualInfo));
	if (!win->client.info) {

#if defined (VERBOSE)
		gf_loge("WINDOW: VisualInfo allocation failed\n");
#endif

		return (false);
	}
	XMatchVisualInfo(win->client.dsp, win->client.screen_id, 24, TrueColor, win->client.info);
	if (!win->client.info) {

#if defined (VERBOSE)
		gf_loge("WINDOW: VisualInfo match failed\n");
#endif

		return (false);
	}

	return (true);
#endif

}

GFAPIS	bool	__gf_createWindow(t_window win, const size_t w, const size_t h) {
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
	
	/* Creating window attributes
	 * */
	memset(&_attr, 0, sizeof(XSetWindowAttributes));
	_attr.colormap = XCreateColormap(win->client.dsp, win->client.root_id, win->client.info->visual, AllocNone);
	_attr.background_pixmap = None;
	_attr.background_pixel = 0;
	_attr.border_pixel = 0;
	_attr.event_mask = _event_mask;

	/* Creating the window itself
	 * */
	win->client.id = XCreateWindow(
		win->client.dsp, win->client.root_id,
		0, 0, w, h, 0,
		win->client.info->depth,
		InputOutput,
		win->client.info->visual,
		CWColormap | CWBorderPixel | CWBackPixel | CWEventMask,
		&_attr
	);
	
	memset(&_hints, 0, sizeof(XWMHints));
	_hints.input = true;
	_hints.flags = InputHint;
	XSetWMHints(win->client.dsp, win->client.id, &_hints);
	XSelectInput(win->client.dsp, win->client.id, _event_mask);
	XMapWindow(win->client.dsp, win->client.id);
	return (true);
}

GFAPIS bool	__gf_processAtoms(t_window win) {
	win->client.atoms.WM_DELETE_WINDOW = XInternAtom(win->client.dsp, "WM_DELETE_WINDOW", false);
	win->client.atoms.WM_CHANGE_STATE = XInternAtom(win->client.dsp, "WM_CHANGE_STATE", false);

	win->client.atoms._MOTIF_WM_HINTS = XInternAtom(win->client.dsp, "_MOTIF_WM_HINTS", false);

	win->client.atoms._NET_WM_STATE = XInternAtom(win->client.dsp, "_NET_WM_STATE", false);
	win->client.atoms._NET_WM_STATE_ABOVE = XInternAtom(win->client.dsp, "_NET_WM_STATE_ABOVE", false);
	win->client.atoms._NET_WM_STATE_FULLSCREEN = XInternAtom(win->client.dsp, "_NET_WM_STATE_FULLSCREEN", false);
	win->client.atoms._NET_WM_STATE_HIDDEN = XInternAtom(win->client.dsp, "_NET_WM_STATE_HIDDEN", false);
	win->client.atoms._NET_WM_STATE_MAXIMIZED_HORZ = XInternAtom(win->client.dsp, "_NET_WM_STATE_MAXIMIZED_HORZ", false);
	win->client.atoms._NET_WM_STATE_MAXIMIZED_VERT = XInternAtom(win->client.dsp, "_NET_WM_STATE_MAXIMIZED_VERT", false);
	win->client.atoms._NET_WM_WINDOW_TYPE = XInternAtom(win->client.dsp, "_NET_WM_WINDOW_TYPE", false);
	win->client.atoms._NET_WM_WINDOW_NORMAL = XInternAtom(win->client.dsp, "_NET_WM_WINDOW_NORMAL", false);
	win->client.atoms._NET_WM_WINDOW_DOCK = XInternAtom(win->client.dsp, "_NET_WM_WINDOW_DOCK", false);

	XSetWMProtocols(win->client.dsp, win->client.id, &win->client.atoms.WM_DELETE_WINDOW, 1);
	return (true);
}
