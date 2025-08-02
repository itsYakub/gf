#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_setWindowResizable(t_window win, bool state) {
	(void) win;
	(void) state;
	return (true);
}

GFAPI bool	gf_setWindowBorderless(t_window win, bool state) {
	(void) win;
	(void) state;
	return (true);
}

GFAPI bool	gf_setWindowFullscreen(t_window win, bool state) {
	(void) win;
	(void) state;
	return (true);
}
