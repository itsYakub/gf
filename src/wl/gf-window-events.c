#if !defined USE_WL
# define USE_WL
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Static globals
 * */

static const struct {
	int32_t	wl;
	int32_t	gf;
} g_button_map[] = {
	{ 1, GF_BUTTON_LEFT },
	{ 2, GF_BUTTON_RIGHT },
	{ 3, GF_BUTTON_MIDDLE },

	{ INT_MAX, GF_BUTTON_NONE }
};

static const struct {
	int32_t	wl;
	int32_t	gf;
	char	*str;
} g_key_map[] = {

	/* SECTION:
	 *  ASCII keys
	 * */

	{ 0, GF_KEY_SPACE, "GF_KEY_SPACE" },

	{ 0, GF_KEY_A, "GF_KEY_A" },
	{ 0, GF_KEY_B, "GF_KEY_B" },
	{ 0, GF_KEY_C, "GF_KEY_C" },
	{ 0, GF_KEY_D, "GF_KEY_D" },
	{ 0, GF_KEY_E, "GF_KEY_E" },
	{ 0, GF_KEY_F, "GF_KEY_F" },
	{ 0, GF_KEY_G, "GF_KEY_G" },
	{ 0, GF_KEY_H, "GF_KEY_H" },
	{ 0, GF_KEY_I, "GF_KEY_I" },
	{ 0, GF_KEY_J, "GF_KEY_J" },
	{ 0, GF_KEY_K, "GF_KEY_K" },
	{ 0, GF_KEY_L, "GF_KEY_L" },
	{ 0, GF_KEY_M, "GF_KEY_M" },
	{ 0, GF_KEY_N, "GF_KEY_N" },
	{ 0, GF_KEY_O, "GF_KEY_O" },
	{ 0, GF_KEY_P, "GF_KEY_P" },
	{ 0, GF_KEY_Q, "GF_KEY_Q" },
	{ 0, GF_KEY_R, "GF_KEY_R" },
	{ 0, GF_KEY_S, "GF_KEY_S" },
	{ 0, GF_KEY_T, "GF_KEY_T" },
	{ 0, GF_KEY_U, "GF_KEY_U" },
	{ 0, GF_KEY_V, "GF_KEY_V" },
	{ 0, GF_KEY_W, "GF_KEY_W" },
	{ 0, GF_KEY_X, "GF_KEY_X" },
	{ 0, GF_KEY_Y, "GF_KEY_Y" },
	{ 0, GF_KEY_Z, "GF_KEY_Z" },
	
	{ 0, GF_KEY_A, "GF_KEY_A" },
	{ 0, GF_KEY_B, "GF_KEY_B" },
	{ 0, GF_KEY_C, "GF_KEY_C" },
	{ 0, GF_KEY_D, "GF_KEY_D" },
	{ 0, GF_KEY_E, "GF_KEY_E" },
	{ 0, GF_KEY_F, "GF_KEY_F" },
	{ 0, GF_KEY_G, "GF_KEY_G" },
	{ 0, GF_KEY_H, "GF_KEY_H" },
	{ 0, GF_KEY_I, "GF_KEY_I" },
	{ 0, GF_KEY_J, "GF_KEY_J" },
	{ 0, GF_KEY_K, "GF_KEY_K" },
	{ 0, GF_KEY_L, "GF_KEY_L" },
	{ 0, GF_KEY_M, "GF_KEY_M" },
	{ 0, GF_KEY_N, "GF_KEY_N" },
	{ 0, GF_KEY_O, "GF_KEY_O" },
	{ 0, GF_KEY_P, "GF_KEY_P" },
	{ 0, GF_KEY_Q, "GF_KEY_Q" },
	{ 0, GF_KEY_R, "GF_KEY_R" },
	{ 0, GF_KEY_S, "GF_KEY_S" },
	{ 0, GF_KEY_T, "GF_KEY_T" },
	{ 0, GF_KEY_U, "GF_KEY_U" },
	{ 0, GF_KEY_V, "GF_KEY_V" },
	{ 0, GF_KEY_W, "GF_KEY_W" },
	{ 0, GF_KEY_X, "GF_KEY_X" },
	{ 0, GF_KEY_Y, "GF_KEY_Y" },
	{ 0, GF_KEY_Z, "GF_KEY_Z" },

	{ 0, GF_KEY_0, "GF_KEY_0" },
	{ 0, GF_KEY_1, "GF_KEY_1" },
	{ 0, GF_KEY_2, "GF_KEY_2" },
	{ 0, GF_KEY_3, "GF_KEY_3" },
	{ 0, GF_KEY_4, "GF_KEY_4" },
	{ 0, GF_KEY_5, "GF_KEY_5" },
	{ 0, GF_KEY_6, "GF_KEY_6" },
	{ 0, GF_KEY_7, "GF_KEY_7" },
	{ 0, GF_KEY_8, "GF_KEY_8" },
	{ 0, GF_KEY_9, "GF_KEY_9" },

	{ 0, GF_KEY_EXCLAM, "GF_KEY_EXCLAM" },
	{ 0, GF_KEY_AT, "GF_KEY_AT" },
	/* { missing, GF_KEY_hash, "GF_KEY_" }, */
	{ 0, GF_KEY_DOLLAR, "GF_KEY_DOLLAR" },
	{ 0, GF_KEY_PERCENT, "GF_KEY_PERCENT" },
	/* { missing, GF_KEY_CARET, "GF_KEY_" }, */
	{ 0, GF_KEY_AMPER, "GF_KEY_AMPER" },
	{ 0, GF_KEY_ASTER, "GF_KEY_ASTER" },
	{ 0, GF_KEY_OPENPAREN, "GF_KEY_OPENPAREN" },
	{ 0, GF_KEY_CLOSEPAREN, "GF_KEY_CLOSEPAREN" },
	{ 0, GF_KEY_EQUAL, "GF_KEY_EQUAL" },
	{ 0, GF_KEY_OPENBRACE, "GF_KEY_OPENBRACE" },
	{ 0, GF_KEY_CLOSEBRACE, "GF_KEY_CLOSEBRACE" },
	{ 0, GF_KEY_BACKSLASH, "GF_KEY_BACKSLASH" },
	{ 0, GF_KEY_SEMICOLON, "GF_KEY_SEMICOLON" },
	/* { missing, GF_KEY_SQUOTE, "GF_KEY_SQUOTE" }, */
	{ 0, GF_KEY_COMMA, "GF_KEY_COMMA" },
	{ 0, GF_KEY_PERIOD, "GF_KEY_PERIOD" },
	{ 0, GF_KEY_SLASH, "GF_KEY_SLASH" },
	{ 0, GF_KEY_GRAVE, "GF_KEY_GRAVE" },
	
	{ 0, GF_KEY_UNDERSCORE, "GF_KEY_UNDERSCORE" },
	{ 0, GF_KEY_PLUS, "GF_KEY_PLUS" },
	/* { missing, GF_KEY_OPENCURLY, "GF_KEY_OPENCURLY" }, */
	/* { missing, GF_KEY_CLOSECURLY, "GF_KEY_CLOSECURLY" }, */
	{ 0, GF_KEY_PIPE, "GF_KEY_PIPE" },
	{ 0, GF_KEY_COLON, "GF_KEY_COLON" },
	/* { missing, GF_KEY_DQUOTE, "GF_KEY_DQUOTE" }, */
	/* { missing, GF_KEY_OPENANGLE, "GF_KEY_OPENANGLE" }, */
	/* { missing, GF_KEY_CLOSEANGLE, "GF_KEY_CLOSEANGLE" }, */
	{ 0, GF_KEY_QUESTION, "GF_KEY_QUOESTION" },
	{ 0, GF_KEY_TILDE, "GF_KEY_TILDE" },

	/* SECTION:
	 *  Functional keys
	 * */

	{ 0, GF_KEY_ENTER, "GF_KEY_ENTER" },
	{ 0, GF_KEY_TAB, "GF_KEY_TAB" },
	{ 0, GF_KEY_BACKSPACE, "GF_KEY_BACKSPACE" },
	{ 0, GF_KEY_CAPSLOCK, "GF_KEY_CAPSLOCK" },
	
	{ 0, GF_KEY_LSHIFT, "GF_KEY_LSHIFT" },
	{ 0, GF_KEY_RSHIFT, "GF_KEY_RSHIFT" },
	{ 0, GF_KEY_LCTRL, "GF_KEY_LCTRL" },
	{ 0, GF_KEY_RCTRL, "GF_KEY_RCTRL" },
	{ 0, GF_KEY_LALT, "GF_KEY_LALT" },
	{ 0, GF_KEY_RALT, "GF_KEY_RALT" },
	{ 0, GF_KEY_SUPER, "GF_KEY_SUPER" },
	{ 0, GF_KEY_SUPER, "GF_KEY_SUPER" },
	
	{ 0, GF_KEY_TOP, "GF_KEY_TOP" },
	{ 0, GF_KEY_DOWN, "GF_KEY_DOWN" },
	{ 0, GF_KEY_LEFT, "GF_KEY_LEFT" },
	{ 0, GF_KEY_RIGHT, "GF_KEY_RIGHT" },

	/* { missing, GF_KEY_FN }, */
	{ 0, GF_KEY_F1, "GF_KEY_F1" },
	{ 0, GF_KEY_F2, "GF_KEY_F2" },
	{ 0, GF_KEY_F3, "GF_KEY_F3" },
	{ 0, GF_KEY_F4, "GF_KEY_F4" },
	{ 0, GF_KEY_F5, "GF_KEY_F5" },
	{ 0, GF_KEY_F6, "GF_KEY_F6" },
	{ 0, GF_KEY_F7, "GF_KEY_F7" },
	{ 0, GF_KEY_F8, "GF_KEY_F8" },
	{ 0, GF_KEY_F9, "GF_KEY_F9" },
	{ 0, GF_KEY_F10, "GF_KEY_F10" },
	{ 0, GF_KEY_F11, "GF_KEY_F11" },
	{ 0, GF_KEY_F12, "GF_KEY_F12" },

	/* SECTION:
	 *  Termination
	 * */

	{ INT_MAX, GF_KEY_NONE, "GF_KEY_NONE" },
};





/* SECTION:
 *  Private interface declarations
 * */

/* * Wayland to gf inputting translation
 * * */
GFAPIS int32_t	__gf_getButtonFromWlButtons(const int32_t);
GFAPIS int32_t	__gf_getKeycodeFromWl1Keysym(const int32_t);

/* * gf internal event processing
 * * */
GFAPIS bool		__gf_pollGfEvents(t_window, t_event *);





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_pollEvents(t_window win, t_event *event) {
	if (__gf_pollGfEvents(win, event)) {
		return (true);
	}
	*event = (t_event) { 0 };
	return (false);
}

GFAPI bool	gf_popEvent(t_window win, t_event *e) {
	if (win->events.cnt <= 0) {
		return (false);
	}
	if (e) {
		memcpy(e, &win->events.lst[win->events.cnt - 1], sizeof(t_event));
	}
	memset(&win->events.lst[--win->events.cnt], 0, sizeof(t_event));
	return (true);
}

GFAPI bool	gf_pushEvent(t_window win, t_event *e) {
	/* Check overflows of event queue
	 * */
	if (win->events.cnt >= _GF_EVENT_QUEUE_SIZE) {
		return (false);
	}
	/* Check if event is valid
	 * */
	if (e->type == GF_EVENT_NONE) {
		return (false);
	}
	memcpy(&win->events.lst[win->events.cnt++], e, sizeof(t_event));
	return (true);
}

GFAPI char	*gf_keyToString(const int32_t key) {
	for (size_t i = 0; g_key_map[i].gf != GF_KEY_NONE; i++) {
		if (g_key_map[i].gf == (int32_t) key) {
			return (g_key_map[i].str);
		}
	}
	return ("(null)");
}





/* SECTION:
 *  Private interface definitions
 * */

/* * Wayland to gf inputting translation
 * * */

GFAPIS int32_t	__gf_getButtonFromWlButtons(const int32_t btn) {
	for (size_t i = 0; g_button_map[i].gf != GF_BUTTON_NONE; i++) {
		if (g_button_map[i].wl == (int32_t) btn) {
			return (g_button_map[i].gf);
		}
	}
	return (GF_KEY_NONE);
}

GFAPIS int32_t	__gf_getKeycodeFromWlKeysym(const int32_t key) {
	for (size_t i = 0; g_key_map[i].gf != GF_KEY_NONE; i++) {
		if (g_key_map[i].wl == (int32_t) key) {
			return (g_key_map[i].gf);
		}
	}
	return (GF_KEY_NONE);
}





/* * gf internal event processing
 * * */

GFAPIS bool	__gf_pollGfEvents(t_window win, t_event *event) {
	/* Poll gf events from event queue
	 * */
	while ((int) win->events.cnt > 0) {
		gf_popEvent(win, event);
		return (true);
	}
	return (false);
}
