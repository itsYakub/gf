#include "./../gf.h"
#include "./gf-int.h"

#include <dlfcn.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GL/glx.h>
#include <GL/glxext.h>

#include <GL/gl.h>

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

static int32_t	g_glx_attr_ctx[] = {
	GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
	GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
	GLX_CONTEXT_MINOR_VERSION_ARB, 3,
	None
};

static PFNGLXCREATECONTEXTATTRIBSARBPROC	glXCreateContextAttribsARB;

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_createContextEGL(t_window);
GFAPIS bool	__gf_createContextGLX(t_window);

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createContext(t_window win) {

#if defined (USE_GLX)
	__gf_createContextGLX(win);
#elif defined (USE_EGL)
	__gf_createContextEGL(win);
#else
	return (false);
#endif

#if defined (VERBOSE)
	gf_logi("GL: Version: %s\n", glGetString(GL_VERSION));
	gf_logi("GL: Vendor: %s\n", glGetString(GL_VENDOR));
	gf_logi("GL: Renderer: %s\n", glGetString(GL_RENDERER));
		
	gf_logi("GLSL: Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

	/* Setting up some default context settings
	 * */
	gf_setWindowVSync(win, 0);
	return (true);
}

GFAPI bool	gf_makeCurrent(t_window win) {
	if (!win->glx.ctx && !win->egl.ctx) {
		return (gf_createContext(win));
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

	return (true);
}

GFAPI bool	gf_swapBuffers(t_window win) {

#if defined (USE_GLX)
	glXSwapBuffers(win->x11.dsp, win->x11.id);
	return (true);
#elif defined (USE_EGL)
	eglSwapBuffers(win->egl.dsp, win->egl.surf);
	return (true);
#endif

	return (false);
}

GFAPI void	*gf_getProcAddress(const char *name) {
	const char	*_names[] = { "libGL.so.1", "libGL.so", 0 };
	void		*_handle;
	void		*_ptr;

	for (size_t i = 0; _names[i]; i++) {
		_handle = dlopen(_names[i], RTLD_NOW | RTLD_GLOBAL);
		if (_handle) {
			_ptr = dlsym(_handle, name);
			if (_ptr) {
				return (_ptr);
			}
		}
	}
	return (0);
}

/* SECTION:
 *  Private interface definitions
 * */

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
}

GFAPIS bool	__gf_createContextGLX(t_window win) {
	glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((GLubyte *) "glXCreateContextAttribsARB");
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

	if (!glXMakeCurrent(win->x11.dsp, win->x11.id, win->glx.ctx)) {

#if defined (VERBOSE)
		gf_loge("GLX: Context failed\n");
#endif

		return (false);
	}

#if defined (VERBOSE)
	gf_logi("GLX: Created successfully\n");
	gf_logi("GLX: Version: %s\n", glXGetClientString(win->x11.dsp, GLX_VERSION));
	gf_logi("GLX: Vendor: %s\n", glXGetClientString(win->x11.dsp, GLX_VENDOR));
#endif

	return (true);
}
