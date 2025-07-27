#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_h_)
# define _gf_h_

/* GFAPI:
 *  keyword for public interface declarations/definitions
 * */

# if !defined (GFAPI)
#  define GFAPI extern 
# endif

/* GFAPIS:
 *  keyword for static/private interface declarations/definitions
 * */

# if !defined (GFAPIS)
#  define GFAPIS static inline
# endif

/* GF_VERSION:
 *  the current up-to-date version of gf
 * GF_VERSION_MAJOR:
 *  the current major version of gf
 * GF_VERSION_MINOR:
 *  the current minor version of gf
 * */
# if !defined (GF_VERSION)
#  define GF_VERSION "1.0"
# endif
# if !defined (GF_VERSION_MAJOR)
#  define GF_VERSION_MAJOR 1
# endif
# if !defined (GF_VERSION)
#  define GF_VERSION_MINOR 0
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
	GF_BUTTON_NONE = 0,
	GF_BUTTON_LEFT = 1,
	GF_BUTTON_RIGHT,
	GF_BUTTON_MIDDLE
} /* Mouse Buttons */;



enum {
	GF_KEY_NONE = 0,
	
	GF_KEY_SPACE = ' ',

	GF_KEY_0 = '0',
	GF_KEY_1, GF_KEY_2, GF_KEY_3,
	GF_KEY_4, GF_KEY_5, GF_KEY_6,
	GF_KEY_7, GF_KEY_8, GF_KEY_9,

	GF_KEY_EXCLAM = '!',
	GF_KEY_AT = '@',
	GF_KEY_HASH = '#',
	GF_KEY_DOLLAR = '$',
	GF_KEY_PERCENT = '%',
	GF_KEY_CARET = '^',
	GF_KEY_AMPER = '&',
	GF_KEY_ASTER = '*',
	GF_KEY_OPENPAREN = '(',
	GF_KEY_CLOSEPAREN = ')',

	GF_KEY_A = 'A',
	GF_KEY_B, GF_KEY_C, GF_KEY_D, GF_KEY_E,	GF_KEY_F,
	GF_KEY_G, GF_KEY_H, GF_KEY_I, GF_KEY_J, GF_KEY_K,
	GF_KEY_L, GF_KEY_M, GF_KEY_N, GF_KEY_O, GF_KEY_P,
	GF_KEY_Q, GF_KEY_R, GF_KEY_S, GF_KEY_T, GF_KEY_U,	
	GF_KEY_V, GF_KEY_W, GF_KEY_X, GF_KEY_Y, GF_KEY_Z,
	
	GF_KEY_MINUS = '-',
	GF_KEY_EQUAL = '=',
	GF_KEY_OPENBRACE = '[',
	GF_KEY_CLOSEBRACE = ']',
	GF_KEY_BACKSLASH = '\\',
	GF_KEY_SEMICOLON = ';',
	GF_KEY_SQUOTE = '\'',
	GF_KEY_COMMA = ',',
	GF_KEY_PERIOD = '.',
	GF_KEY_SLASH = '/',
	GF_KEY_GRAVE = '`',
	
	GF_KEY_UNDERSCORE = '_',
	GF_KEY_PLUS = '+',
	GF_KEY_OPENCURLY = '{',
	GF_KEY_CLOSECURLY = '}',
	GF_KEY_PIPE = '|',
	GF_KEY_COLON = ':',
	GF_KEY_DQUOTE = '\"',
	GF_KEY_OPENANGLE = '<',
	GF_KEY_CLOSEANGLE = '>',
	GF_KEY_QUESTION = '?',
	GF_KEY_TILDE = '~',

	GF_KEY_RETURN = 128,

	GF_KEY_ENTER = 256,
	GF_KEY_TAB,
	GF_KEY_BACKSPACE,
	GF_KEY_CAPSLOCK,

	GF_KEY_LSHIFT, GF_KEY_RSHIFT,
	GF_KEY_LCTRL, GF_KEY_RCTRL,
	GF_KEY_LALT, GF_KEY_RALT,
	GF_KEY_SUPER,

	GF_KEY_TOP, GF_KEY_DOWN,
	GF_KEY_LEFT, GF_KEY_RIGHT,

	GF_KEY_FN,
	GF_KEY_F1, GF_KEY_F2, GF_KEY_F3,
	GF_KEY_F4, GF_KEY_F5, GF_KEY_F6,
	GF_KEY_F7, GF_KEY_F8, GF_KEY_F9,
	GF_KEY_F10, GF_KEY_F11, GF_KEY_F12,

	/* ...More keycodes put here...
	 * */
	GF_KEYCODE_COUNT
} /* Keycodes */;



enum {
	GF_EVENT_NONE = 0,
	GF_EVENT_QUIT = 1,		/* DATA: none */
	GF_EVENT_RESIZE,		/* DATA: [0] -> width, [1] -> height */
	GF_EVENT_MOVE,			/* DATA: [0] -> x, [1] -> y */
	GF_EVENT_MOUSEMOTION,	/* DATA: [0] -> x, [1] -> y, [2] -> x (relative), [3] -> y (relative) */
	GF_EVENT_MOUSE_PRESS,	/* DATA: [0] -> button pressed, [1] -> state */
	GF_EVENT_MOUSE_RELEASE,	/* DATA: [0] -> button released, [1] -> state */
	GF_EVENT_KEY_PRESS,		/* DATA: [0] -> key pressed, [1] -> state */
	GF_EVENT_KEY_RELEASE,	/* DATA: [0] -> key released, [1] -> state */
	/* ...More events put here...
	 * */
	GF_EVENT_COUNT
} /* Window events */;



struct s_event {
	int32_t	type;
	int32_t	data[8];
};

typedef struct s_event	t_event;



/* Opaque pointer left for the implementation to fill
 * */

typedef struct s_window	*t_window;





/* SECTION:
 *  Interface declarations
 * */

GFAPI bool	gf_createWindow(t_window *, const size_t, const size_t, const char *);
GFAPI bool	gf_destroyWindow(t_window);

GFAPI bool	gf_makeCurrent(t_window);

GFAPI bool	gf_swapBuffers(t_window);

GFAPI bool	gf_pollEvents(t_window, t_event *);
GFAPI bool	gf_popEvent(t_window, t_event *);
GFAPI bool	gf_pushEvent(t_window, t_event *);

GFAPI bool	gf_getWindowSize(t_window, int32_t *, int32_t *);
GFAPI bool	gf_getMonitorSize(t_window, int32_t *, int32_t *);
GFAPI bool	gf_getWindowPosition(t_window, int32_t *, int32_t *);

GFAPI bool	gf_setWindowResizable(t_window, bool);
GFAPI bool	gf_setWindowBorderless(t_window, bool);
GFAPI bool	gf_setWindowTopMost(t_window, bool);
GFAPI bool	gf_setWindowMinimized(t_window, bool);
GFAPI bool	gf_setWindowMaximized(t_window, bool);
GFAPI bool	gf_setWindowFullscreen(t_window, bool);
GFAPI bool	gf_setWindowVSync(t_window, bool);

GFAPI void	*gf_getProcAddress(const char *);

#endif
