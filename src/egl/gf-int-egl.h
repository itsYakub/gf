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

struct s_EGL {
	void	*handle;
	struct {
	} /* EGL API */;
};

/* SECTION:
 *  Internal function declarations
 * */

GFAPII struct s_EGL	*gf_int_EGL(void);
GFAPII bool			gf_int_initPlatformEGL(void);

#endif
