#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyContext(t_window win) {
	eglMakeCurrent(win->egl.dsp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(win->egl.dsp, win->egl.ctx);
	eglDestroySurface(win->egl.dsp, win->egl.surf);
	eglTerminate(win->egl.dsp);
	return (true);
}
