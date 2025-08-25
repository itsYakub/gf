#include "gf-int-x11.h"
#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

/* TODO(yakub):
 *  Implement those
 * */

GFAPI bool	gf_clipCopy(t_window win, const char *str, size_t siz) {
	return (true);
}

GFAPI bool	gf_clipPaste(t_window win, char **strptr, size_t *sizptr) {
	return (true);
}
