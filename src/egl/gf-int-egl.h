#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_egl_h_)
# define _gf_int_egl_h_

# if !defined USE_EGL
#  define USE_EGL
# endif
# include "./../gf.h"
# include <EGL/egl.h>
# include <EGL/eglext.h>

/* SECTION:
 *  Type definitions
 * */

struct s_context {
	EGLDisplay	dsp;
	EGLConfig	conf;
	EGLSurface	surf;
	EGLContext	ctx;
};

typedef EGLDisplay	(*PFN_eglGetDisplay) (EGLNativeDisplayType);
typedef EGLBoolean	(*PFN_eglInitialize) (EGLDisplay, EGLint *, EGLint *);
typedef EGLBoolean	(*PFN_eglMakeCurrent) (EGLDisplay, EGLSurface, EGLSurface, EGLContext);
typedef EGLBoolean	(*PFN_eglBindAPI) (EGLenum);
typedef EGLBoolean	(*PFN_eglChooseConfig) (EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *);
typedef EGLSurface	(*PFN_eglCreateWindowSurface) (EGLDisplay, EGLConfig, EGLNativeWindowType, const EGLint *);
typedef EGLContext	(*PFN_eglCreateContext) (EGLDisplay, EGLConfig, EGLContext, const EGLint *);
typedef EGLBoolean	(*PFN_eglDestroyContext) (EGLDisplay, EGLContext);
typedef EGLBoolean	(*PFN_eglTerminate) (EGLDisplay);
typedef EGLBoolean	(*PFN_eglDestroySurface) (EGLDisplay, EGLSurface);
typedef EGLBoolean	(*PFN_eglSwapBuffers) (EGLDisplay, EGLSurface);
typedef EGLBoolean	(*PFN_eglSwapInterval) (EGLDisplay, EGLint);

struct s_EGL {
	void	*handle;
	struct {
		PFN_eglGetDisplay			eglGetDisplay;
		PFN_eglInitialize			eglInitialize;
		PFN_eglMakeCurrent			eglMakeCurrent;
		PFN_eglBindAPI				eglBindAPI;
		PFN_eglChooseConfig			eglChooseConfig;
		PFN_eglCreateWindowSurface	eglCreateWindowSurface;
		PFN_eglCreateContext		eglCreateContext;
		PFN_eglDestroyContext		eglDestroyContext;
		PFN_eglTerminate			eglTerminate;
		PFN_eglDestroySurface		eglDestroySurface;
		PFN_eglSwapBuffers			eglSwapBuffers;
		PFN_eglSwapInterval			eglSwapInterval;
	} /* EGL API */;
};

extern struct s_EGL	g_EGL;

/* SECTION:
 *  Internal function declarations
 * */

GFAPII bool	gf_int_initPlatformEGL(void);

#endif
