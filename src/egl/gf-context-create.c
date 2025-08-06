#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

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
 *  Public API implementation
 * */

GFAPI bool	gf_createContext(t_window win) {
	EGLNativeDisplayType	_dsp;
	EGLNativeWindowType		_win;
	int32_t					_attr_cont_cnt;
	int32_t					_major;
	int32_t					_minor;

#if defined (USE_X11)
	_dsp = (EGLNativeDisplayType) win->x11.dsp;
	_win = (EGLNativeWindowType) win->x11.id;
#elif defined (USE_WL)
	_dsp = (EGLNativeDisplayType) win->wl.dsp;
	_win = (EGLNativeWindowType) win->wl.id;
#endif

	/* Safety check if context is already created
	 * */
	if (win->egl.ctx) {
		return (true);
	}

	/* Create EGL display object based on platform display
	 * */
	win->egl.dsp = eglGetDisplay(_dsp);
	if (win->egl.dsp == EGL_NO_DISPLAY) {

#if defined (VERBOSE)
		gf_loge("EGL: Display failed\n");
#endif

		return (false);
	}
	if (!eglInitialize(win->egl.dsp, &_major, &_minor)) {

#if defined (VERBOSE)
		gf_loge("EGL: Initialization failed | code:%d\n", eglGetError());
#endif

		return (false);
	}
	
	if (!eglBindAPI(EGL_OPENGL_API)) {

#if defined (VERBOSE)
		gf_loge("EGL: Failed to bind OpenGL api\n");
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

	/* Create EGL surface based on platform window
	 * */
	win->egl.surf = eglCreateWindowSurface(win->egl.dsp, win->egl.conf, _win, 0);
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

	if (!eglMakeCurrent(win->egl.dsp, win->egl.surf, win->egl.surf, win->egl.ctx)) {

#if defined (VERBOSE)
		gf_loge("EGL: Context failed\n");
#endif

		return (false);
	}
	return (true);
}

GFAPI bool	gf_makeCurrent(t_window win) {
	if (!win->egl.ctx) {
		return (gf_createContext(win));
	}

	if (!eglMakeCurrent(win->egl.dsp, win->egl.surf, win->egl.surf, win->egl.ctx)) {

# if defined (VERBOSE)
		gf_loge("EGL: Context current failed\n");
# endif

		return (false);
	}

	return (true);
}
