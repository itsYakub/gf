#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_destroyWindow(t_window win) {
	if (win->client.id) XDestroyWindow(win->client.dsp, win->client.id);
	if (win->client.info) free(win->client.info);
	if (win->client.dsp) XCloseDisplay(win->client.dsp);

	free(win);
	return (true);
}
