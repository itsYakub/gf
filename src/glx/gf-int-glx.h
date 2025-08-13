#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_glx_h_)
# define _gf_int_glx_h_

# if !defined USE_GLX
#  define USE_GLX
# endif
# include "./../gf.h"
# include <GL/glx.h>
# include <GL/glxext.h>

/* SECTION:
 *  Type definitions
 * */

struct s_context {
	GLXFBConfig	conf;
	GLXContext	id;
};

#endif
