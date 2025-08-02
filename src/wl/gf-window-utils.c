#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

GFAPI bool	gf_getWindowSize(t_window win, int32_t *wptr, int32_t *hptr) {
	(void) win;
	(void) wptr;
	(void) hptr;
	return (true);
}

GFAPI bool	gf_getMonitorSize(t_window win, int32_t *wptr, int32_t *hptr) {
	(void) win;
	(void) wptr;
	(void) hptr;
	return (true);
}

GFAPI bool	gf_getWindowPosition(t_window win, int32_t *xptr, int32_t *yptr) {
	(void) win;
	(void) xptr;
	(void) yptr;
	return (true);
}
