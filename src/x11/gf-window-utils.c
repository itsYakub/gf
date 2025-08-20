#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Internal API implementation
 * */

GFAPII bool	gf_int_safetyCheckX11(struct s_client *ctx) {
	return (ctx->id != 0);
}
