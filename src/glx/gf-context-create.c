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

static int32_t	g_glx_attr_ctx[] = {
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
		win->glx.ctx = glXCreateContextAttribsARB(win->x11.dsp, win->glx.conf, 0, 1, g_glx_attr_ctx);
		if (!win->glx.ctx) {
			return (false);
		}
	}
	else {
	win->glx.ctx = glXCreateContext(win->x11.dsp, win->x11.info, 0, 1);
		if (!win->glx.ctx) {
			return (false);
		}
	}

	if (!glXMakeCurrent(win->x11.dsp, win->x11.id, win->glx.ctx)) {

#if defined (VERBOSE)
		gf_loge("GLX: Context failed\n");
#endif

		return (false);
	}

#if defined (VERBOSE)
	gf_logi("GLX: Created successfully\n");
	gf_logi("GLX: Version: %s\n", glXGetClientString(win->x11.dsp, GLX_VERSION));
	gf_logi("GLX: Vendor: %s\n", glXGetClientString(win->x11.dsp, GLX_VENDOR));
#endif

	return (true);
}

GFAPI bool	gf_makeCurrent(t_window win) {
	if (!win->glx.ctx) {
		return (gf_createContext(win));
	}

	if (!glXMakeCurrent(win->x11.dsp, win->x11.id, win->glx.ctx)) {

# if defined (VERBOSE)
		gf_loge("GLX: Context current failed\n");
# endif

		return (false);
	}

	return (true);
}
