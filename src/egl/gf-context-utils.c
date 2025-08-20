#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Internal API implementation
 * */

GFAPII bool	gf_int_safetyCheckEGL(struct s_context *ctx) {
	return (ctx->dsp != 0 && ctx->surf != 0 && ctx->conf != 0 && ctx->ctx != 0);
}
