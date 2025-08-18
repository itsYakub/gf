#if !defined (USE_EGL)
# define USE_EGL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_swapBuffers(t_window win) {
	g_EGL.eglSwapBuffers(win->context.dsp, win->context.surf);
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
	g_EGL.eglSwapInterval(win->context.dsp, state);
	return (true);
}
