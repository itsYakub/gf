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
	{ BTN_LEFT, GF_BUTTON_LEFT },
	{ BTN_RIGHT, GF_BUTTON_RIGHT },
	{ BTN_MIDDLE, GF_BUTTON_MIDDLE },

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

	{ XKB_KEY_space, GF_KEY_SPACE, "GF_KEY_SPACE" },

	{ XKB_KEY_a, GF_KEY_A, "GF_KEY_A" },
	{ XKB_KEY_b, GF_KEY_B, "GF_KEY_B" },
	{ XKB_KEY_c, GF_KEY_C, "GF_KEY_C" },
	{ XKB_KEY_d, GF_KEY_D, "GF_KEY_D" },
	{ XKB_KEY_e, GF_KEY_E, "GF_KEY_E" },
	{ XKB_KEY_f, GF_KEY_F, "GF_KEY_F" },
	{ XKB_KEY_g, GF_KEY_G, "GF_KEY_G" },
	{ XKB_KEY_h, GF_KEY_H, "GF_KEY_H" },
	{ XKB_KEY_i, GF_KEY_I, "GF_KEY_I" },
	{ XKB_KEY_j, GF_KEY_J, "GF_KEY_J" },
	{ XKB_KEY_k, GF_KEY_K, "GF_KEY_K" },
	{ XKB_KEY_l, GF_KEY_L, "GF_KEY_L" },
	{ XKB_KEY_m, GF_KEY_M, "GF_KEY_M" },
	{ XKB_KEY_n, GF_KEY_N, "GF_KEY_N" },
	{ XKB_KEY_o, GF_KEY_O, "GF_KEY_O" },
	{ XKB_KEY_p, GF_KEY_P, "GF_KEY_P" },
	{ XKB_KEY_q, GF_KEY_Q, "GF_KEY_Q" },
	{ XKB_KEY_r, GF_KEY_R, "GF_KEY_R" },
	{ XKB_KEY_s, GF_KEY_S, "GF_KEY_S" },
	{ XKB_KEY_t, GF_KEY_T, "GF_KEY_T" },
	{ XKB_KEY_u, GF_KEY_U, "GF_KEY_U" },
	{ XKB_KEY_v, GF_KEY_V, "GF_KEY_V" },
	{ XKB_KEY_w, GF_KEY_W, "GF_KEY_W" },
	{ XKB_KEY_x, GF_KEY_X, "GF_KEY_X" },
	{ XKB_KEY_y, GF_KEY_Y, "GF_KEY_Y" },
	{ XKB_KEY_z, GF_KEY_Z, "GF_KEY_Z" },
	
	{ XKB_KEY_A, GF_KEY_A, "GF_KEY_A" },
	{ XKB_KEY_B, GF_KEY_B, "GF_KEY_B" },
	{ XKB_KEY_C, GF_KEY_C, "GF_KEY_C" },
	{ XKB_KEY_D, GF_KEY_D, "GF_KEY_D" },
	{ XKB_KEY_E, GF_KEY_E, "GF_KEY_E" },
	{ XKB_KEY_F, GF_KEY_F, "GF_KEY_F" },
	{ XKB_KEY_G, GF_KEY_G, "GF_KEY_G" },
	{ XKB_KEY_H, GF_KEY_H, "GF_KEY_H" },
	{ XKB_KEY_I, GF_KEY_I, "GF_KEY_I" },
	{ XKB_KEY_J, GF_KEY_J, "GF_KEY_J" },
	{ XKB_KEY_K, GF_KEY_K, "GF_KEY_K" },
	{ XKB_KEY_L, GF_KEY_L, "GF_KEY_L" },
	{ XKB_KEY_M, GF_KEY_M, "GF_KEY_M" },
	{ XKB_KEY_N, GF_KEY_N, "GF_KEY_N" },
	{ XKB_KEY_O, GF_KEY_O, "GF_KEY_O" },
	{ XKB_KEY_P, GF_KEY_P, "GF_KEY_P" },
	{ XKB_KEY_Q, GF_KEY_Q, "GF_KEY_Q" },
	{ XKB_KEY_R, GF_KEY_R, "GF_KEY_R" },
	{ XKB_KEY_S, GF_KEY_S, "GF_KEY_S" },
	{ XKB_KEY_T, GF_KEY_T, "GF_KEY_T" },
	{ XKB_KEY_U, GF_KEY_U, "GF_KEY_U" },
	{ XKB_KEY_V, GF_KEY_V, "GF_KEY_V" },
	{ XKB_KEY_W, GF_KEY_W, "GF_KEY_W" },
	{ XKB_KEY_X, GF_KEY_X, "GF_KEY_X" },
	{ XKB_KEY_Y, GF_KEY_Y, "GF_KEY_Y" },
	{ XKB_KEY_Z, GF_KEY_Z, "GF_KEY_Z" },

	{ XKB_KEY_0, GF_KEY_0, "GF_KEY_0" },
	{ XKB_KEY_1, GF_KEY_1, "GF_KEY_1" },
	{ XKB_KEY_2, GF_KEY_2, "GF_KEY_2" },
	{ XKB_KEY_3, GF_KEY_3, "GF_KEY_3" },
	{ XKB_KEY_4, GF_KEY_4, "GF_KEY_4" },
	{ XKB_KEY_5, GF_KEY_5, "GF_KEY_5" },
	{ XKB_KEY_6, GF_KEY_6, "GF_KEY_6" },
	{ XKB_KEY_7, GF_KEY_7, "GF_KEY_7" },
	{ XKB_KEY_8, GF_KEY_8, "GF_KEY_8" },
	{ XKB_KEY_9, GF_KEY_9, "GF_KEY_9" },

	{ XKB_KEY_exclam, GF_KEY_EXCLAM, "GF_KEY_EXCLAM" },
	{ XKB_KEY_at, GF_KEY_AT, "GF_KEY_AT" },
	{ XKB_KEY_numbersign, GF_KEY_HASH, "GF_KEY_HASH" },
	{ XKB_KEY_dollar, GF_KEY_DOLLAR, "GF_KEY_DOLLAR" },
	{ XKB_KEY_percent, GF_KEY_PERCENT, "GF_KEY_PERCENT" },
	{ XKB_KEY_caret, GF_KEY_CARET, "GF_KEY_CARET" },
	{ XKB_KEY_ampersand, GF_KEY_AMPER, "GF_KEY_AMPER" },
	{ XKB_KEY_asterisk, GF_KEY_ASTER, "GF_KEY_ASTER" },
	{ XKB_KEY_parenleft, GF_KEY_OPENPAREN, "GF_KEY_OPENPAREN" },
	{ XKB_KEY_parenright, GF_KEY_CLOSEPAREN, "GF_KEY_CLOSEPAREN" },
	{ XKB_KEY_equal, GF_KEY_EQUAL, "GF_KEY_EQUAL" },
	{ XKB_KEY_bracketleft, GF_KEY_OPENBRACE, "GF_KEY_OPENBRACE" },
	{ XKB_KEY_bracketright, GF_KEY_CLOSEBRACE, "GF_KEY_CLOSEBRACE" },
	{ XKB_KEY_backslash, GF_KEY_BACKSLASH, "GF_KEY_BACKSLASH" },
	{ XKB_KEY_semicolon, GF_KEY_SEMICOLON, "GF_KEY_SEMICOLON" },
	{ XKB_KEY_quoteleft, GF_KEY_SQUOTE, "GF_KEY_SQUOTE" },
	{ XKB_KEY_comma, GF_KEY_COMMA, "GF_KEY_COMMA" },
	{ XKB_KEY_period, GF_KEY_PERIOD, "GF_KEY_PERIOD" },
	{ XKB_KEY_slash, GF_KEY_SLASH, "GF_KEY_SLASH" },
	{ XKB_KEY_grave, GF_KEY_GRAVE, "GF_KEY_GRAVE" },
	
	{ XKB_KEY_underscore, GF_KEY_UNDERSCORE, "GF_KEY_UNDERSCORE" },
	{ XKB_KEY_plus, GF_KEY_PLUS, "GF_KEY_PLUS" },
	{ XKB_KEY_braceleft, GF_KEY_OPENCURLY, "GF_KEY_OPENCURLY" },
	{ XKB_KEY_braceright, GF_KEY_CLOSECURLY, "GF_KEY_CLOSECURLY" },
	{ XKB_KEY_bar, GF_KEY_PIPE, "GF_KEY_PIPE" },
	{ XKB_KEY_colon, GF_KEY_COLON, "GF_KEY_COLON" },
	{ XKB_KEY_quotedbl, GF_KEY_DQUOTE, "GF_KEY_DQUOTE" },
	{ XKB_KEY_less, GF_KEY_OPENANGLE, "GF_KEY_OPENANGLE" },
	{ XKB_KEY_greater, GF_KEY_CLOSEANGLE, "GF_KEY_CLOSEANGLE" },
	{ XKB_KEY_question, GF_KEY_QUESTION, "GF_KEY_QUOESTION" },
	{ XKB_KEY_ntilde, GF_KEY_TILDE, "GF_KEY_TILDE" },

	/* SECTION:
	 *  Functional keys
	 * */

	{ XKB_KEY_Return, GF_KEY_ENTER, "GF_KEY_ENTER" },
	{ XKB_KEY_Escape, GF_KEY_ESCAPE, "GF_KEY_ESCAPE" },
	{ XKB_KEY_Tab, GF_KEY_TAB, "GF_KEY_TAB" },
	{ XKB_KEY_BackSpace, GF_KEY_BACKSPACE, "GF_KEY_BACKSPACE" },
	{ XKB_KEY_Caps_Lock, GF_KEY_CAPSLOCK, "GF_KEY_CAPSLOCK" },
	
	{ XKB_KEY_Shift_L, GF_KEY_LSHIFT, "GF_KEY_LSHIFT" },
	{ XKB_KEY_Shift_R, GF_KEY_RSHIFT, "GF_KEY_RSHIFT" },
	{ XKB_KEY_Control_L, GF_KEY_LCTRL, "GF_KEY_LCTRL" },
	{ XKB_KEY_Control_R, GF_KEY_RCTRL, "GF_KEY_RCTRL" },
	{ XKB_KEY_Alt_L, GF_KEY_LALT, "GF_KEY_LALT" },
	{ XKB_KEY_Alt_R, GF_KEY_RALT, "GF_KEY_RALT" },
	{ XKB_KEY_Super_R, GF_KEY_SUPER, "GF_KEY_SUPER" },
	{ XKB_KEY_Super_L, GF_KEY_SUPER, "GF_KEY_SUPER" },
	
	{ XKB_KEY_Up, GF_KEY_TOP, "GF_KEY_TOP" },
	{ XKB_KEY_Down, GF_KEY_DOWN, "GF_KEY_DOWN" },
	{ XKB_KEY_Left, GF_KEY_LEFT, "GF_KEY_LEFT" },
	{ XKB_KEY_Right, GF_KEY_RIGHT, "GF_KEY_RIGHT" },

	/* { missing, GF_KEY_FN, "GF_KEY_FN" }, */
	{ XKB_KEY_F1, GF_KEY_F1, "GF_KEY_F1" },
	{ XKB_KEY_F2, GF_KEY_F2, "GF_KEY_F2" },
	{ XKB_KEY_F3, GF_KEY_F3, "GF_KEY_F3" },
	{ XKB_KEY_F4, GF_KEY_F4, "GF_KEY_F4" },
	{ XKB_KEY_F5, GF_KEY_F5, "GF_KEY_F5" },
	{ XKB_KEY_F6, GF_KEY_F6, "GF_KEY_F6" },
	{ XKB_KEY_F7, GF_KEY_F7, "GF_KEY_F7" },
	{ XKB_KEY_F8, GF_KEY_F8, "GF_KEY_F8" },
	{ XKB_KEY_F9, GF_KEY_F9, "GF_KEY_F9" },
	{ XKB_KEY_F10, GF_KEY_F10, "GF_KEY_F10" },
	{ XKB_KEY_F11, GF_KEY_F11, "GF_KEY_F11" },
	{ XKB_KEY_F12, GF_KEY_F12, "GF_KEY_F12" },

	/* SECTION:
	 *  Termination
	 * */

	{ INT_MAX, GF_KEY_NONE, "GF_KEY_NONE" },
};





/* SECTION:
 *  Private interface declarations
 * */

GFAPIS bool		__gf_pollGfEvents(t_window, t_event *);
GFAPIS bool		__gf_pollInternalEvents(t_window);





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_pollEvents(t_window win, t_event *event) {
	/* Poll gf events
	 * */
	if (__gf_pollGfEvents(win, event)) {
		return (true);
	}

	/* Poll internal events
	 * */
	__gf_pollInternalEvents(win);

	/* Return an empty 'None' event
	 * */
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

GFAPI bool	gf_flushEvents(t_window win) {
	struct pollfd	_fd;

	_fd = (struct pollfd) {
		.fd = wl_display_get_fd(win->wl.dsp), .events = POLLOUT
	};
	while (wl_display_flush(win->wl.dsp) == -1) {
		if (errno == EAGAIN) {
			return (false);
		}
		while (poll(&_fd, 1, -1) == -1) {
			if (errno != EINTR && errno != EAGAIN) {
				return (false);
			}
		}
	}
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

GFAPII int32_t	gf_int_buttonPlatformToGf(const int32_t btn) {
	for (size_t i = 0; g_button_map[i].gf != GF_BUTTON_NONE; i++) {
		if (g_button_map[i].wl == (int32_t) btn) {
			return (g_button_map[i].gf);
		}
	}
	return (GF_KEY_NONE);
}

GFAPII int32_t	gf_int_keymapPlatformToGf(const int32_t key) {
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

GFAPIS bool	__gf_pollInternalEvents(t_window win) {
	bool			_event;
	enum { POLLFD_DISPLAY = 0 };
	struct pollfd	_fd[2] = {
		[POLLFD_DISPLAY] = { .fd = wl_display_get_fd(win->wl.dsp), .events = POLLIN },
	};
	_event = false;
	while (!_event) {
		/* Prepare event reading
		 * */
		while (wl_display_prepare_read(win->wl.dsp) != 0) {
			if (wl_display_dispatch_pending(win->wl.dsp) > 0) {
				return (false);
			}
		}
		if (!gf_flushEvents(win)) {
			wl_display_cancel_read(win->wl.dsp);
			return (false);
		}
		if (poll(_fd, 1, -1) > 0) {
			wl_display_cancel_read(win->wl.dsp);
			return (false);
		}

		/* Process events
		 * */

		/* * Display events
		 * * */
		if (_fd[POLLFD_DISPLAY].revents & POLLIN) {
			wl_display_read_events(win->wl.dsp);
			if (wl_display_dispatch_pending(win->wl.dsp) > 0) {
				_event = true;
			}
		}
		else {
			wl_display_cancel_read(win->wl.dsp);
		}
	}
	return (true);
}
