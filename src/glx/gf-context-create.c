#if !defined (USE_GLX) && !defined(USE_X11)
# define USE_X11
# define USE_GLX
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Static globals
 * */

static PFNGLXCREATECONTEXTATTRIBSARBPROC	glXCreateContextAttribsARB = 0;

static int32_t	g_client_attr_ctx[] = {
	GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
	GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
	GLX_CONTEXT_MINOR_VERSION_ARB, 3,
	None
};





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_createContext(t_window win) {
	glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((GLubyte *) "glXCreateContextAttribsARB");
	if (glXCreateContextAttribsARB) {
		win->context.id = glXCreateContextAttribsARB(win->client.dsp, win->context.conf, 0, 1, g_client_attr_ctx);
		if (!win->context.id) {

#if defined (VERBOSE)
			gf_loge("CONTEXT: Failed to create\n");
#endif

			return (false);
		}
	}
	else {
	win->context.id = glXCreateContext(win->client.dsp, win->client.info, 0, 1);
		if (!win->context.id) {

#if defined (VERBOSE)
			gf_loge("CONTEXT: Failed to create\n");
#endif

			return (false);
		}
	}
	return (gf_makeCurrent(win));
}

GFAPI bool	gf_makeCurrent(t_window win) {
	if (!win->context.id) {
		return (gf_createContext(win));
	}

	if (!glXMakeCurrent(win->client.dsp, win->client.id, win->context.id)) {

# if defined (VERBOSE)
		gf_loge("CONTEXT: Make current failed\n");
# endif

		return (false);
	}

	return (true);
}
