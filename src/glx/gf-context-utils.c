#if !defined (USE_GLX) && !defined(USE_X11)
# define USE_X11
# define USE_GLX
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Internal API implementation
 * */

GFAPII bool	gf_int_safetyCheckGLX(struct s_context *ctx) {
	return (ctx->id != 0);
}
