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
	if (!gf_int_safetyCheckGLX(&win->context)) return (false);

	if (win->context.id) g_GLX.glXDestroyContext(win->client.dsp, win->context.id);
	return (true);
}
