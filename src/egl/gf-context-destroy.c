#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyContext(t_window win) {
	eglMakeCurrent(win->context.dsp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	if (win->context.ctx) eglDestroyContext(win->context.dsp, win->context.ctx);
	if (win->context.surf) eglDestroySurface(win->context.dsp, win->context.surf);
	if (win->context.ctx) eglTerminate(win->context.dsp);
	return (true);
}
