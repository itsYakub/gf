#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_h_)
# define _gf_int_h_

# include "./gf.h"
# define _GF_EVENT_QUEUE_SIZE	64
# define _GF_WINDOW_TITLE_LEN	1024

# if defined (USE_EGL)
#  include "./egl/gf-int-egl.h"
# elif defined (USE_GLX)
#  include "./glx/gf-int-glx.h"
# endif
# include <GL/gl.h>

# if defined (USE_X11)
#  include "./x11/gf-int-x11.h"
# elif defined (USE_WL)
#  include "./wl/gf-int-wl.h"
# endif

struct s_window {
	struct s_client		client;
	struct s_context	context;

	struct {
		t_event	lst[_GF_EVENT_QUEUE_SIZE];
		size_t	cnt;
	} events;

	struct {
		int32_t	id;
	} config;

	struct {
		char	title[_GF_WINDOW_TITLE_LEN + 1];

		int32_t	width;
		int32_t	height;

		int32_t	x;
		int32_t	y;
	} data;
};

typedef struct s_window	*t_window;



/* SECTION:
 *  Interface
 * */

GFAPII int32_t	gf_int_buttonPlatformToGf(const int32_t);
GFAPII int32_t	gf_int_keymapPlatformToGf(const int32_t);

# include <errno.h>
# include <string.h>
# include <limits.h>
# include <assert.h>
# include <stdlib.h>
# if defined (__linux__)
#  include <poll.h>
#  include <dlfcn.h>
#  include <unistd.h>
#  include <sys/mman.h>
#  include <linux/input-event-codes.h>
# endif
#endif
