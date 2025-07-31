#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyWindow(t_window win) {
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
	
	free(win);
	return (true);
}
