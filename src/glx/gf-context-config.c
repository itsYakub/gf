#if !defined (USE_GLX) && !defined(USE_X11)
# define USE_X11
# define USE_GLX
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  *  Static globals
 *   * */

static PFNGLXSWAPINTERVALEXTPROC	glXSwapIntervalEXT = 0;





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_swapBuffers(t_window win) {
	glXSwapBuffers(win->client.dsp, win->client.id);
	return (true);
}

GFAPI void	*gf_getProcAddress(const char *name) {
	const char	*_names[] = { "libGL.so.1", "libGL.so", 0 };
	void		*_handle;
	void		*_ptr;

	for (size_t i = 0; _names[i]; i++) {
		_handle = dlopen(_names[i], RTLD_NOW | RTLD_GLOBAL);
		if (_handle) {
			_ptr = dlsym(_handle, name);
			if (_ptr) {
				return (_ptr);
			}
		}
	}
	return (0);
}

GFAPI bool	gf_setWindowVSync(t_window win, bool state) {
	if (!glXSwapIntervalEXT) {
		glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((GLubyte *) "glXSwapIntervalEXT");
		if (!glXSwapIntervalEXT) {
			return (false);
		}
	}
	glXSwapIntervalEXT(win->client.dsp, win->client.id, state);
	return (true);
}
