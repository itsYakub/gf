#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_h_)
# define _gh_h_

/* GFAPI:
 *  keyword for public interface declarations/definitions
 * */

# if !defined GFAPI
#  define GFAPI extern 
# endif

/* GFAPIS:
 *  keyword for static/private interface declarations/definitions
 * */

# if !defined GFAPIS
#  define GFAPIS static inline
# endif

/* Standard-library includes
 * */

# include <stdio.h>
# include <stddef.h>
# include <stdint.h>
# include <string.h>
# if !defined (__cplusplus)
#  include <stdbool.h>
# endif

/* Simple terminal logging
 * NOTE(yakub):
 *  I intend logging to be only a part of the debug builds.
 *  Release builds shoul leave logging to the user and if error happens,
 *  it should be stored in some errno - like global variable / window member variable.
 * (edit):
 *  This was a stupid idea: built-in logging should only occur if there's VERBOSE flag defined in the Makefile
 *  (or in other words: in the final build of the library).
 *  Yes, I'm stupid. How could you tell?
 * */

# if !defined gf_logi
#  define gf_logi(...) fprintf(stdout, "\e[1;32m[ INFO ] \e[0m" __VA_ARGS__)
# endif
# if !defined gf_logw
#  define gf_logw(...) fprintf(stdout, "\e[1;33m[ WARN ] \e[0m" __VA_ARGS__)
# endif
# if !defined gf_loge
#  define gf_loge(...) fprintf(stdout, "\e[1;31m[ ERROR ] \e[0m" __VA_ARGS__)
# endif

enum {
	GF_BUTTON_LEFT = 1,
	GF_BUTTON_RIGHT,
	GF_BUTTON_MIDDLE
} /* Mouse Buttons */;

enum {
	GF_WINDOW_FLAGS_NONE = 0,
	GF_WINDOW_RESIZABLE = 1,
	GF_WINDOW_VSYNC_HINT,
	GF_WINDOW_FLAGS_COUNT
} /* Window states */;

enum {
	GF_EVENT_NONE = 0,
	GF_EVENT_QUIT = 1,		/* DATA: none */
	GF_EVENT_RESIZE,		/* DATA: i: [0] -> width, [1] -> height */
	GF_EVENT_MOVE,			/* DATA: i: [0] -> x, [1] -> y */
	GF_EVENT_MOUSEMOTION,	/* DATA: i: [0] -> x, [1] -> y, [2] -> x (relative), [3] -> y (relative) */
	GF_EVENT_MOUSE_PRESS,	/* DATA: c: [0] -> button pressed, [1] -> state */
	GF_EVENT_MOUSE_RELEASE,	/* DATA: c: [0] -> button released, [1] -> state */
	/* ...More events put here...
	 * */
	GF_EVENT_COUNT
} /* Window events */;

struct s_event {
	int32_t	type;
	struct {
		int8_t	c[8];
		int32_t	i[8];
		int64_t	l[8];
	} s_data;
};

typedef struct s_event	t_event;

/* Opaque pointer left for the implementation to fill
 * */

typedef struct s_window	*t_window;

/* SECTION:
 *  Interface declarations
 * */

GFAPI bool	gf_createWindow(t_window *, const size_t, const size_t, const char *, const int32_t);
GFAPI bool	gf_destroyWindow(t_window);

GFAPI bool	gf_swapBuffers(t_window);

GFAPI bool	gf_pollEvents(t_window, t_event *);
GFAPI bool	gf_popEvent(t_window, t_event *);
GFAPI bool	gf_pushEvent(t_window, t_event *);

GFAPI bool	gf_getWindowSize(t_window, int32_t *, int32_t *);
GFAPI bool	gf_getWindowPosition(t_window, int32_t *, int32_t *);

GFAPI void	*gf_getProcAddress(const char *);

#endif
