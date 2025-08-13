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
	if (win->context.id) glXDestroyContext(win->client.dsp, win->context.id);
	return (true);
}
