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
	int32_t	_attr_cont_cnt;

	/* Create EGL display object based on X11 object
	 * */

#if defined (USE_X11)
	win->egl.dsp = eglGetDisplay((EGLNativeDisplayType) win->x11.dsp);
#elif defined (USE_WL)
	win->egl.dsp = eglGetDisplay((EGLNativeDisplayType) 0);
#endif

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

	/* Create EGL surface based on X11 window
	 * */

#if defined (USE_X11)
	win->egl.surf = eglCreateWindowSurface(win->egl.dsp, win->egl.conf, (EGLNativeWindowType) win->x11.id, 0);
#elif defined (USE_WL)
	win->egl.surf = eglCreateWindowSurface(win->egl.dsp, win->egl.conf, (EGLNativeWindowType) 0, 0);
#endif

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

#if defined (VERBOSE)
	gf_logi("EGL: Created successfully\n");
	gf_logi("EGL: Client: %s\n", eglQueryString(win->egl.dsp, EGL_CLIENT_APIS));
	gf_logi("EGL: Version: %s\n", eglQueryString(win->egl.dsp, EGL_VERSION));
	gf_logi("EGL: Vendor: %s\n", eglQueryString(win->egl.dsp, EGL_VENDOR));
#endif

	return (true);

	/* Setting up some default context settings
	 * */
	gf_setWindowVSync(win, 0);
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
