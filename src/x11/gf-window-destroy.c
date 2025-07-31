#include "./../gf.h"
#include "./gf-int.h"

#include <assert.h>
#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <GL/glx.h>
#include <GL/glxext.h>

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool	__gf_destroyWindow(t_window);
GFAPIS bool	__gf_destroyContext(t_window);

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyWindow(t_window win) {
	gf_int_ensureWindow(win);
	assert(__gf_destroyContext(win));
	assert(__gf_destroyWindow(win));
	free(win);
	return (true);
}

/* SECTION:
 *  Private interface definitions
 * */

GFAPIS	bool	__gf_destroyWindow(t_window win) {
	XDestroyWindow(win->x11.dsp, win->x11.id);

#if defined (VERBOSE)
	gf_logi("WINDOW: Destroyed successfully\n");
#endif

	free(win->x11.info);

#if defined (VERBOSE)
	gf_logi("VISUAL: Freed successfully\n");
#endif

	XCloseDisplay(win->x11.dsp);

#if defined (VERBOSE)
	gf_logi("DISPLAY: Destroyed successfully\n");
#endif
	return (true);
}

GFAPIS	bool	__gf_destroyContext(t_window win) {

#if defined (USE_GLX)
	glXDestroyContext(win->x11.dsp, win->glx.ctx);

# if defined (VERBOSE)
	gf_logi("GLX: Terminated successfully\n");
# endif

		return (true);
#elif defined (USE_EGL)
	eglMakeCurrent(win->egl.dsp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(win->egl.dsp, win->egl.ctx);
	eglDestroySurface(win->egl.dsp, win->egl.surf);
	eglTerminate(win->egl.dsp);

# if defined (VERBOSE)
	gf_logi("EGL: Terminated successfully\n");
# endif

	return (true);
#endif

	return (false);
}
