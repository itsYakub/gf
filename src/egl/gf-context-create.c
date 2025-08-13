#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Static globals
 * */

static int32_t	g_context_attr_conf[] = {
	EGL_RED_SIZE,			8,
	EGL_GREEN_SIZE,			8,
	EGL_BLUE_SIZE,			8,
	EGL_ALPHA_SIZE,			8,
	EGL_DEPTH_SIZE,			24,
	EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
	EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
	EGL_NONE
};

static int32_t	g_context_attr_ctx[] = {
	EGL_CONTEXT_CLIENT_VERSION,	2,
	EGL_NONE
};





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createContext(t_window win) {
	EGLNativeDisplayType	_dsp;
	EGLNativeWindowType		_win;
	int32_t					_attr_cont_cnt;
	int32_t					_major;
	int32_t					_minor;

#if defined (USE_X11)
	_dsp = (EGLNativeDisplayType) win->client.dsp;
	_win = (EGLNativeWindowType) win->client.id;
#elif defined (USE_WL)
	_dsp = (EGLNativeDisplayType) win->client.dsp;
	_win = (EGLNativeWindowType) win->client.id;
#endif

	/* Safety check if context is already created
	 * */
	if (win->context.ctx) {
		return (true);
	}

	/* Create EGL display object based on platform display
	 * */
	win->context.dsp = eglGetDisplay(_dsp);
	if (win->context.dsp == EGL_NO_DISPLAY) {

#if defined (VERBOSE)
		gf_loge("CONTEXT: Display failed\n");
#endif

		return (false);
	}
	if (!eglInitialize(win->context.dsp, &_major, &_minor)) {

#if defined (VERBOSE)
		gf_loge("CONTEXT: Initialization failed\n");
#endif

		return (false);
	}
	
	if (!eglBindAPI(EGL_OPENGL_API)) {

#if defined (VERBOSE)
		gf_loge("CONTEXT: Failed to bind OpenGL api\n");
#endif

		return (false);
	}

	/* Create config object
	 * */
	_attr_cont_cnt = 0;
	if (!eglChooseConfig(win->context.dsp, g_context_attr_conf, &win->context.conf, 1, &_attr_cont_cnt)) {

#if defined (VERBOSE)
		gf_loge("CONTEXT: Configuration failed\n");
#endif

		return (false);
	}

	/* Create EGL surface based on platform window
	 * */
	win->context.surf = eglCreateWindowSurface(win->context.dsp, win->context.conf, _win, 0);
	if (!win->context.surf) {

#if defined (VERBOSE)
		gf_loge("CONTEXT: Surface failed\n");
#endif

		return (false);
	}

	/* Create EGL Context
	 * */
	win->context.ctx = eglCreateContext(win->context.dsp, win->context.conf, EGL_NO_CONTEXT, g_context_attr_ctx);
	if (!win->context.ctx) {

#if defined (VERBOSE)
		gf_loge("CONTEXT: Failed to create\n");
#endif

		return (false);
	}

	return (gf_makeCurrent(win));
}

GFAPI bool	gf_makeCurrent(t_window win) {
	if (!win->context.ctx) {
		return (gf_createContext(win));
	}

	if (!eglMakeCurrent(win->context.dsp, win->context.surf, win->context.surf, win->context.ctx)) {

# if defined (VERBOSE)
		gf_loge("CONTEXT: Make current failed\n");
# endif

		return (false);
	}

	return (true);
}
