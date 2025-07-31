#if !defined (USE_GLX) && !defined(USE_X11)
# define USE_X11
# define USE_GLX
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyContext(t_window win) {
	glXDestroyContext(win->x11.dsp, win->glx.ctx);

# if defined (VERBOSE)
	gf_logi("EGL: Terminated successfully\n");
# endif

	return (true);
}
