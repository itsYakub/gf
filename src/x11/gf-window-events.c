#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Static globals
 * */

static const struct {
	int32_t	x11;
	int32_t	gf;
} g_button_map[] = {
	{ 1, GF_BUTTON_LEFT },
	{ 2, GF_BUTTON_RIGHT },
	{ 3, GF_BUTTON_MIDDLE },

	{ INT_MAX, GF_BUTTON_NONE }
};

static const struct {
	int32_t	x11;
	int32_t	gf;
	char	*str;
} g_key_map[] = {

	/* SECTION:
	 *  ASCII keys
	 * */

	{ XK_space, GF_KEY_SPACE, "GF_KEY_SPACE" },

	{ XK_a, GF_KEY_A, "GF_KEY_A" },
	{ XK_b, GF_KEY_B, "GF_KEY_B" },
	{ XK_c, GF_KEY_C, "GF_KEY_C" },
	{ XK_d, GF_KEY_D, "GF_KEY_D" },
	{ XK_e, GF_KEY_E, "GF_KEY_E" },
	{ XK_f, GF_KEY_F, "GF_KEY_F" },
	{ XK_g, GF_KEY_G, "GF_KEY_G" },
	{ XK_h, GF_KEY_H, "GF_KEY_H" },
	{ XK_i, GF_KEY_I, "GF_KEY_I" },
	{ XK_j, GF_KEY_J, "GF_KEY_J" },
	{ XK_k, GF_KEY_K, "GF_KEY_K" },
	{ XK_l, GF_KEY_L, "GF_KEY_L" },
	{ XK_m, GF_KEY_M, "GF_KEY_M" },
	{ XK_n, GF_KEY_N, "GF_KEY_N" },
	{ XK_o, GF_KEY_O, "GF_KEY_O" },
	{ XK_p, GF_KEY_P, "GF_KEY_P" },
	{ XK_q, GF_KEY_Q, "GF_KEY_Q" },
	{ XK_r, GF_KEY_R, "GF_KEY_R" },
	{ XK_s, GF_KEY_S, "GF_KEY_S" },
	{ XK_t, GF_KEY_T, "GF_KEY_T" },
	{ XK_u, GF_KEY_U, "GF_KEY_U" },
	{ XK_v, GF_KEY_V, "GF_KEY_V" },
	{ XK_w, GF_KEY_W, "GF_KEY_W" },
	{ XK_x, GF_KEY_X, "GF_KEY_X" },
	{ XK_y, GF_KEY_Y, "GF_KEY_Y" },
	{ XK_z, GF_KEY_Z, "GF_KEY_Z" },
	
	{ XK_A, GF_KEY_A, "GF_KEY_A" },
	{ XK_B, GF_KEY_B, "GF_KEY_B" },
	{ XK_C, GF_KEY_C, "GF_KEY_C" },
	{ XK_D, GF_KEY_D, "GF_KEY_D" },
	{ XK_E, GF_KEY_E, "GF_KEY_E" },
	{ XK_F, GF_KEY_F, "GF_KEY_F" },
	{ XK_G, GF_KEY_G, "GF_KEY_G" },
	{ XK_H, GF_KEY_H, "GF_KEY_H" },
	{ XK_I, GF_KEY_I, "GF_KEY_I" },
	{ XK_J, GF_KEY_J, "GF_KEY_J" },
	{ XK_K, GF_KEY_K, "GF_KEY_K" },
	{ XK_L, GF_KEY_L, "GF_KEY_L" },
	{ XK_M, GF_KEY_M, "GF_KEY_M" },
	{ XK_N, GF_KEY_N, "GF_KEY_N" },
	{ XK_O, GF_KEY_O, "GF_KEY_O" },
	{ XK_P, GF_KEY_P, "GF_KEY_P" },
	{ XK_Q, GF_KEY_Q, "GF_KEY_Q" },
	{ XK_R, GF_KEY_R, "GF_KEY_R" },
	{ XK_S, GF_KEY_S, "GF_KEY_S" },
	{ XK_T, GF_KEY_T, "GF_KEY_T" },
	{ XK_U, GF_KEY_U, "GF_KEY_U" },
	{ XK_V, GF_KEY_V, "GF_KEY_V" },
	{ XK_W, GF_KEY_W, "GF_KEY_W" },
	{ XK_X, GF_KEY_X, "GF_KEY_X" },
	{ XK_Y, GF_KEY_Y, "GF_KEY_Y" },
	{ XK_Z, GF_KEY_Z, "GF_KEY_Z" },

	{ XK_0, GF_KEY_0, "GF_KEY_0" },
	{ XK_1, GF_KEY_1, "GF_KEY_1" },
	{ XK_2, GF_KEY_2, "GF_KEY_2" },
	{ XK_3, GF_KEY_3, "GF_KEY_3" },
	{ XK_4, GF_KEY_4, "GF_KEY_4" },
	{ XK_5, GF_KEY_5, "GF_KEY_5" },
	{ XK_6, GF_KEY_6, "GF_KEY_6" },
	{ XK_7, GF_KEY_7, "GF_KEY_7" },
	{ XK_8, GF_KEY_8, "GF_KEY_8" },
	{ XK_9, GF_KEY_9, "GF_KEY_9" },

	{ XK_exclam, GF_KEY_EXCLAM, "GF_KEY_EXCLAM" },
	{ XK_at, GF_KEY_AT, "GF_KEY_AT" },
	/* { missing, GF_KEY_hash, "GF_KEY_" }, */
	{ XK_dollar, GF_KEY_DOLLAR, "GF_KEY_DOLLAR" },
	{ XK_percent, GF_KEY_PERCENT, "GF_KEY_PERCENT" },
	/* { missing, GF_KEY_CARET, "GF_KEY_" }, */
	{ XK_ampersand, GF_KEY_AMPER, "GF_KEY_AMPER" },
	{ XK_asterisk, GF_KEY_ASTER, "GF_KEY_ASTER" },
	{ XK_parenleft, GF_KEY_OPENPAREN, "GF_KEY_OPENPAREN" },
	{ XK_parenright, GF_KEY_CLOSEPAREN, "GF_KEY_CLOSEPAREN" },
	
	{ XK_minus, GF_KEY_MINUS, "GF_KEY_MINUS" },
	{ XK_equal, GF_KEY_EQUAL, "GF_KEY_EQUAL" },
	{ XK_braceleft, GF_KEY_OPENBRACE, "GF_KEY_OPENBRACE" },
	{ XK_braceright, GF_KEY_CLOSEBRACE, "GF_KEY_CLOSEBRACE" },
	{ XK_backslash, GF_KEY_BACKSLASH, "GF_KEY_BACKSLASH" },
	{ XK_semicolon, GF_KEY_SEMICOLON, "GF_KEY_SEMICOLON" },
	/* { missing, GF_KEY_SQUOTE, "GF_KEY_SQUOTE" }, */
	{ XK_comma, GF_KEY_COMMA, "GF_KEY_COMMA" },
	{ XK_period, GF_KEY_PERIOD, "GF_KEY_PERIOD" },
	{ XK_slash, GF_KEY_SLASH, "GF_KEY_SLASH" },
	{ XK_grave, GF_KEY_GRAVE, "GF_KEY_GRAVE" },
	
	{ XK_underscore, GF_KEY_UNDERSCORE, "GF_KEY_UNDERSCORE" },
	{ XK_plus, GF_KEY_PLUS, "GF_KEY_PLUS" },
	/* { missing, GF_KEY_OPENCURLY, "GF_KEY_OPENCURLY" }, */
	/* { missing, GF_KEY_CLOSECURLY, "GF_KEY_CLOSECURLY" }, */
	{ XK_bar, GF_KEY_PIPE, "GF_KEY_PIPE" },
	{ XK_colon, GF_KEY_COLON, "GF_KEY_COLON" },
	/* { missing, GF_KEY_DQUOTE, "GF_KEY_DQUOTE" }, */
	/* { missing, GF_KEY_OPENANGLE, "GF_KEY_OPENANGLE" }, */
	/* { missing, GF_KEY_CLOSEANGLE, "GF_KEY_CLOSEANGLE" }, */
	{ XK_question, GF_KEY_QUESTION, "GF_KEY_QUOESTION" },
	{ XK_asciitilde, GF_KEY_TILDE, "GF_KEY_TILDE" },

	/* SECTION:
	 *  Functional keys
	 * */

	{ XK_Return, GF_KEY_ENTER, "GF_KEY_ENTER" },
	{ XK_Escape, GF_KEY_ESCAPE, "GF_KEY_ESCAPE" },
	{ XK_Tab, GF_KEY_TAB, "GF_KEY_TAB" },
	{ XK_BackSpace, GF_KEY_BACKSPACE, "GF_KEY_BACKSPACE" },
	{ XK_Caps_Lock, GF_KEY_CAPSLOCK, "GF_KEY_CAPSLOCK" },
	
	{ XK_Shift_L, GF_KEY_LSHIFT, "GF_KEY_LSHIFT" },
	{ XK_Shift_R, GF_KEY_RSHIFT, "GF_KEY_RSHIFT" },
	{ XK_Control_L, GF_KEY_LCTRL, "GF_KEY_LCTRL" },
	{ XK_Control_R, GF_KEY_RCTRL, "GF_KEY_RCTRL" },
	{ XK_Alt_L, GF_KEY_LALT, "GF_KEY_LALT" },
	{ XK_Alt_R, GF_KEY_RALT, "GF_KEY_RALT" },
	{ XK_Super_L, GF_KEY_SUPER, "GF_KEY_SUPER" },
	{ XK_Super_R, GF_KEY_SUPER, "GF_KEY_SUPER" },
	
	{ XK_Up, GF_KEY_TOP, "GF_KEY_TOP" },
	{ XK_Down, GF_KEY_DOWN, "GF_KEY_DOWN" },
	{ XK_Left, GF_KEY_LEFT, "GF_KEY_LEFT" },
	{ XK_Right, GF_KEY_RIGHT, "GF_KEY_RIGHT" },

	/* { missing, GF_KEY_FN }, */
	{ XK_F1, GF_KEY_F1, "GF_KEY_F1" },
	{ XK_F2, GF_KEY_F2, "GF_KEY_F2" },
	{ XK_F3, GF_KEY_F3, "GF_KEY_F3" },
	{ XK_F4, GF_KEY_F4, "GF_KEY_F4" },
	{ XK_F5, GF_KEY_F5, "GF_KEY_F5" },
	{ XK_F6, GF_KEY_F6, "GF_KEY_F6" },
	{ XK_F7, GF_KEY_F7, "GF_KEY_F7" },
	{ XK_F8, GF_KEY_F8, "GF_KEY_F8" },
	{ XK_F9, GF_KEY_F9, "GF_KEY_F9" },
	{ XK_F10, GF_KEY_F10, "GF_KEY_F10" },
	{ XK_F11, GF_KEY_F11, "GF_KEY_F11" },
	{ XK_F12, GF_KEY_F12, "GF_KEY_F12" },

	/* SECTION:
	 *  Termination
	 * */

	{ INT_MAX, GF_KEY_NONE, "GF_KEY_NONE" },
};





/* SECTION:
 *  Private interface declarations
 * */

/* * gf internal event processing
 * * */
GFAPIS bool		__gf_pollGfEvents(t_window, t_event *);

/* * X11 internal event processing
 * * */
GFAPIS bool		__gf_pollInternalEvents(t_window);
GFAPIS bool		__gf_pollInternal_Client(t_window, XEvent *);
GFAPIS bool		__gf_pollInternal_Property(t_window, XEvent *);
GFAPIS bool		__gf_pollInternal_Configure(t_window, XEvent *);
GFAPIS bool		__gf_pollInternal_Mouse(t_window, XEvent *);
GFAPIS bool		__gf_pollInternal_Key(t_window, XEvent *);





/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_pollEvents(t_window win, t_event *event) {
	if (__gf_pollGfEvents(win, event)) {
		return (true);
	}
	__gf_pollInternalEvents(win);
	/* As there was no event to be retrieved, we return the 'none' event
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
	if (!XFlush(win->x11.dsp)) {
		return (false);
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

/* * X11 to gf inputting translation
 * * */

GFAPII int32_t	gf_int_buttonPlatformToGf(const int32_t btn) {
	for (size_t i = 0; g_button_map[i].gf != GF_BUTTON_NONE; i++) {
		if (g_button_map[i].x11 == (int32_t) btn) {
			return (g_button_map[i].gf);
		}
	}
	return (GF_KEY_NONE);
}

GFAPII int32_t	gf_int_keymapPlatformToGf(const int32_t key) {
	for (size_t i = 0; g_key_map[i].gf != GF_KEY_NONE; i++) {
		if (g_key_map[i].x11 == (int32_t) key) {
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





/* * X11 internal event processing
 * * */

GFAPIS bool	__gf_pollInternalEvents(t_window win) {
	XEvent	_event;

	/* Event queue emtpy, now we need to poll the events from implementation
	 * */
	while (XPending(win->x11.dsp)) {
		XNextEvent(win->x11.dsp, &_event);
		switch (_event.type) {
			case (ClientMessage): {
				__gf_pollInternal_Client(win, &_event);
			} break;

			case (PropertyNotify): {
				__gf_pollInternal_Property(win, &_event);
			} break;
			
			case (ConfigureNotify): {
				__gf_pollInternal_Configure(win, &_event);
			} break;	

			case (MotionNotify):	
			case (ButtonPress):
			case (ButtonRelease): {
				__gf_pollInternal_Mouse(win, &_event);
			} break;

			case (KeyPress):
			case (KeyRelease): {
				__gf_pollInternal_Key(win, &_event);
			} break;
		}
	}
	return (true);
}

GFAPIS bool	__gf_pollInternal_Client(t_window win, XEvent *e) {
	t_event	_event;

	if ((Atom) e->xclient.data.l[0] == win->atoms.wm_delete_window) {
		_event.type = _event.quit.type = GF_EVENT_QUIT;
		return (gf_pushEvent(win, &_event));
	}
	return (false);
}

/* NOTE(yakub):
 *  This function is barely tested due to me being on i3wm which is kind of problematic if it comes to property changes
 * */
GFAPIS bool	__gf_pollInternal_Property(t_window win, XEvent *e) {
	t_event		_event;
	Atom		_actual_type;
	int32_t		_actual_format;
	uint64_t	_nitems;
	uint64_t	_bytes;
	uint8_t		*_prop;

	/* This variable is kept for API consistency
	 * */
	(void) e;
	_event = (t_event) { 0 };
	if (XGetWindowProperty(
			win->x11.dsp, win->x11.id, win->atoms.wm_net_state,
			0L, sizeof (Atom), false,
			AnyPropertyType, &_actual_type, &_actual_format,
			&_nitems, &_bytes, &_prop) == Success && _prop
	) {
		Atom	*_atoms;

		_atoms = (Atom *) _prop;
		if (_atoms[0] == win->atoms.wm_net_state_fullscreen) {
			_event = (t_event) { .type = GF_EVENT_FULLSCREEN, .maximize.state = true };
		}
		free(_prop);
	}
	return (gf_pushEvent(win, &_event));
}

GFAPIS bool	__gf_pollInternal_Configure(t_window win, XEvent *e) {
	t_event	_event;

	/* GF_EVENT_RESIZE
	 * */
	if (
		win->misc.width != e->xconfigure.width ||
		win->misc.height != e->xconfigure.height
	) {
		_event.type = GF_EVENT_RESIZE;
		_event.resize.w = win->misc.width = e->xconfigure.width;
		_event.resize.h = win->misc.height = e->xconfigure.height;
	}
	/* GF_EVENT_MOVE
	 * */
	else if (
		win->misc.x != e->xconfigure.x ||
		win->misc.y != e->xconfigure.y
	) {
		_event.type = GF_EVENT_MOVE;
		_event.move.x = win->misc.x = e->xconfigure.x;
		_event.move.y = win->misc.y = e->xconfigure.y;
	}
	else {
		return (false);
	}
	return (gf_pushEvent(win, &_event));
}

GFAPIS bool		__gf_pollInternal_Mouse(t_window win, XEvent *e) {
	t_event	_event;

	/* GF_EVENT_MOUSEMOTION
	 * */
	if (e->type == MotionNotify) {
		_event.type = GF_EVENT_MOUSEMOTION;
		_event.motion.x = e->xmotion.x;
		_event.motion.y = e->xmotion.y;
		_event.motion.xrel = e->xmotion.x_root;
		_event.motion.yrel = e->xmotion.y_root;
	}
	else {
		/* GF_EVENT_MOUSE_SCROLL
		 * */
		if (e->xbutton.button > 3) {
			_event.type = GF_EVENT_MOUSE_SCROLL;
			if (e->xbutton.button == 4) {
				_event.scroll.val = -1;
			}
			else if (e->xbutton.button == 5) {
				_event.scroll.val = 1;
			}
			else {
				_event.scroll.val = 0;
			}
		}
		/* GF_EVENT_MOUSE_PRESS / GF_EVENT_MOUSE_RELEASE
		 * */
		else {
			_event.type = e->type == ButtonPress ? GF_EVENT_MOUSE_PRESS : GF_EVENT_MOUSE_RELEASE;
			_event.button.state = e->type == ButtonPress ? true : false;
			_event.button.btn = gf_int_buttonPlatformToGf(e->xbutton.button);
			if (_event.button.btn == GF_KEY_NONE) {
				return (false);
			}
		}
	}
	return (gf_pushEvent(win, &_event));
}

GFAPIS bool	__gf_pollInternal_Key(t_window win, XEvent *e) {
	t_event	_event;
	int32_t	_key;

	_key = XkbKeycodeToKeysym(win->x11.dsp, e->xkey.keycode, 0, e->xkey.state & ShiftMask ? 1 : 0);
	_event.type = e->type == KeyPress ? GF_EVENT_KEY_PRESS : GF_EVENT_KEY_RELEASE;
	_event.key.state = e->type == KeyPress ? true : false; 
	_event.key.key = gf_int_keymapPlatformToGf(_key);
	if (_event.key.key == GF_KEY_NONE) {
		return (false);
	}
	return (gf_pushEvent(win, &_event));
}
