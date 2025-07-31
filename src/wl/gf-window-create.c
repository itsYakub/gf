#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createWindow(t_window *win, const size_t w, const size_t h, const char *t) {
	*win = (t_window) calloc(1, sizeof(struct s_window));
	
	gf_setWindowResizable(*win, false);
	gf_setWindowBorderless(*win, false);
	gf_setWindowFullscreen(*win, false);

	return (true);
}
