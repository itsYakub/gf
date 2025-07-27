#include "./../gf.h"
#include "./gf-int.h"

#include <limits.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

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
} g_keysym_map[] = {
	/* SECTION:
	 *  ASCII keys
	 * */

	{ XK_space, GF_KEY_SPACE },

	{ XK_a, GF_KEY_A },
	{ XK_b, GF_KEY_B },
	{ XK_c, GF_KEY_C },
	{ XK_d, GF_KEY_D },
	{ XK_e, GF_KEY_E },
	{ XK_f, GF_KEY_F },
	{ XK_g, GF_KEY_G },
	{ XK_h, GF_KEY_H },
	{ XK_i, GF_KEY_I },
	{ XK_j, GF_KEY_J },
	{ XK_k, GF_KEY_K },
	{ XK_l, GF_KEY_L },
	{ XK_m, GF_KEY_M },
	{ XK_n, GF_KEY_N },
	{ XK_o, GF_KEY_O },
	{ XK_p, GF_KEY_P },
	{ XK_q, GF_KEY_Q },
	{ XK_r, GF_KEY_R },
	{ XK_s, GF_KEY_S },
	{ XK_t, GF_KEY_T },
	{ XK_u, GF_KEY_U },
	{ XK_v, GF_KEY_V },
	{ XK_w, GF_KEY_W },
	{ XK_x, GF_KEY_X },
	{ XK_y, GF_KEY_Y },
	{ XK_z, GF_KEY_Z },
	
	{ XK_A, GF_KEY_A },
	{ XK_B, GF_KEY_B },
	{ XK_C, GF_KEY_C },
	{ XK_D, GF_KEY_D },
	{ XK_E, GF_KEY_E },
	{ XK_F, GF_KEY_F },
	{ XK_G, GF_KEY_G },
	{ XK_H, GF_KEY_H },
	{ XK_I, GF_KEY_I },
	{ XK_J, GF_KEY_J },
	{ XK_K, GF_KEY_K },
	{ XK_L, GF_KEY_L },
	{ XK_M, GF_KEY_M },
	{ XK_N, GF_KEY_N },
	{ XK_O, GF_KEY_O },
	{ XK_P, GF_KEY_P },
	{ XK_Q, GF_KEY_Q },
	{ XK_R, GF_KEY_R },
	{ XK_S, GF_KEY_S },
	{ XK_T, GF_KEY_T },
	{ XK_U, GF_KEY_U },
	{ XK_V, GF_KEY_V },
	{ XK_W, GF_KEY_W },
	{ XK_X, GF_KEY_X },
	{ XK_Y, GF_KEY_Y },
	{ XK_Z, GF_KEY_Z },

	{ XK_0, GF_KEY_0 },
	{ XK_1, GF_KEY_1 },
	{ XK_2, GF_KEY_2 },
	{ XK_3, GF_KEY_3 },
	{ XK_4, GF_KEY_4 },
	{ XK_5, GF_KEY_5 },
	{ XK_6, GF_KEY_6 },
	{ XK_7, GF_KEY_7 },
	{ XK_8, GF_KEY_8 },
	{ XK_9, GF_KEY_9 },

	{ XK_exclam, GF_KEY_EXCLAM },
	{ XK_at, GF_KEY_AT },
	/* { missing, GF_KEY_hash }, */
	{ XK_dollar, GF_KEY_DOLLAR },
	{ XK_percent, GF_KEY_PERCENT },
	/* { missing, GF_KEY_CARET }, */
	{ XK_ampersand, GF_KEY_AMPER },
	{ XK_asterisk, GF_KEY_ASTER },
	{ XK_parenleft, GF_KEY_OPENPAREN },
	{ XK_parenright, GF_KEY_CLOSEPAREN },
	
	{ XK_minus, GF_KEY_MINUS },
	{ XK_equal, GF_KEY_EQUAL },
	{ XK_braceleft, GF_KEY_OPENBRACE },
	{ XK_braceright, GF_KEY_CLOSEBRACE },
	{ XK_backslash, GF_KEY_BACKSLASH },
	{ XK_semicolon, GF_KEY_SEMICOLON },
	/* { missing, GF_KEY_SQUOTE }, */
	{ XK_comma, GF_KEY_COMMA },
	{ XK_period, GF_KEY_PERIOD },
	{ XK_slash, GF_KEY_SLASH },
	{ XK_grave, GF_KEY_GRAVE },
	
	{ XK_underscore, GF_KEY_UNDERSCORE },
	{ XK_plus, GF_KEY_PLUS },
	/* { missing, GF_KEY_OPENCURLY }, */
	/* { missing, GF_KEY_CLOSECURLY }, */
	{ XK_bar, GF_KEY_PIPE },
	{ XK_colon, GF_KEY_COLON },
	/* { missing, GF_KEY_DQUOTE }, */
	/* { missing, GF_KEY_OPENANGLE }, */
	/* { missing, GF_KEY_CLOSEANGLE }, */
	{ XK_question, GF_KEY_QUESTION },
	{ XK_asciitilde, GF_KEY_TILDE },

	/* SECTION:
	 *  Functional keys
	 * */

	{ XK_Return, GF_KEY_ENTER },
	{ XK_Tab, GF_KEY_TAB },
	{ XK_BackSpace, GF_KEY_BACKSPACE },
	{ XK_Caps_Lock, GF_KEY_CAPSLOCK },
	
	{ XK_Shift_L, GF_KEY_LSHIFT },
	{ XK_Shift_R, GF_KEY_RSHIFT },
	{ XK_Control_L, GF_KEY_LCTRL },
	{ XK_Control_R, GF_KEY_RCTRL },
	{ XK_Alt_L, GF_KEY_LALT },
	{ XK_Alt_R, GF_KEY_RALT },
	{ XK_Super_L, GF_KEY_SUPER },
	{ XK_Super_R, GF_KEY_SUPER },
	
	{ XK_Up, GF_KEY_TOP },
	{ XK_Down, GF_KEY_DOWN },
	{ XK_Left, GF_KEY_LEFT },
	{ XK_Right, GF_KEY_RIGHT },

	/* { missing, GF_KEY_FN }, */
	{ XK_F1, GF_KEY_F1 },
	{ XK_F2, GF_KEY_F2 },
	{ XK_F3, GF_KEY_F3 },
	{ XK_F4, GF_KEY_F4 },
	{ XK_F5, GF_KEY_F5 },
	{ XK_F6, GF_KEY_F6 },
	{ XK_F7, GF_KEY_F7 },
	{ XK_F8, GF_KEY_F8 },
	{ XK_F9, GF_KEY_F9 },
	{ XK_F10, GF_KEY_F10 },
	{ XK_F11, GF_KEY_F11 },
	{ XK_F12, GF_KEY_F12 },


	/* SECTION:
	 *  Termination
	 * */
	{ INT_MAX, GF_KEY_NONE },
};

/* SECTION:
 *  Private interface declarations
 * */

GFAPIS int32_t	__gf_getButtonFromX11Buttons(const int32_t);
GFAPIS int32_t	__gf_getKeycodeFromX11Keysym(const int32_t);

/* SECTION:
 *  Public API implementation
 * */

GFAPI bool	gf_pollEvents(t_window win, t_event *event) {
	XEvent	_x11_event;
	t_event	_gf_event;

	/* Poll gf events from event queue
	 * */
	{
		while ((int) win->events.cnt > 0) {
			*event = win->events.lst[win->events.cnt - 1];
			gf_popEvent(win, event);
			return (true);
		}
	}

	/* Event queue emtpy, now we need to poll the events from implementation
	 * */
	{
		while (XPending(win->x11.dsp)) {
			memset(&_gf_event, 0, sizeof(t_event));
			XNextEvent(win->x11.dsp, &_x11_event);
			switch (_x11_event.type) {
				case (ClientMessage): {
					if ((Atom) _x11_event.xclient.data.l[0] == win->atoms.wm_delete_window) {
						_gf_event.type = GF_EVENT_QUIT;
						gf_pushEvent(win, &_gf_event);
					}
				} break;
				
				case (ConfigureNotify): {
					if (
						win->misc.width != _x11_event.xconfigure.width ||
						win->misc.height != _x11_event.xconfigure.height
					) {
						_gf_event.type = GF_EVENT_RESIZE;
						_gf_event.data[0] = win->misc.width = _x11_event.xconfigure.width;
						_gf_event.data[1] = win->misc.height = _x11_event.xconfigure.height;
					}
					else if (
						win->misc.x != _x11_event.xconfigure.x ||
						win->misc.y != _x11_event.xconfigure.y
					) {
						_gf_event.type = GF_EVENT_MOVE;
						_gf_event.data[0] = win->misc.x = _x11_event.xconfigure.x;
						_gf_event.data[1] = win->misc.y = _x11_event.xconfigure.y;
					}
					else { break; }
					gf_pushEvent(win, &_gf_event);
				} break;	

				case (MotionNotify): {
					_gf_event.type = GF_EVENT_MOUSEMOTION;
					_gf_event.data[0] = _x11_event.xmotion.x;
					_gf_event.data[1] = _x11_event.xmotion.y;
					_gf_event.data[2] = _x11_event.xmotion.x_root;
					_gf_event.data[3] = _x11_event.xmotion.y_root;
					gf_pushEvent(win, &_gf_event);
				} break;
				
				case (ButtonPress):
				case (ButtonRelease): {
					_gf_event.data[0] = __gf_getButtonFromX11Buttons(_x11_event.xbutton.button);
					if (_gf_event.data[0] == GF_KEY_NONE) {
						break;
					}
					_gf_event.type = _x11_event.type == ButtonPress ? GF_EVENT_MOUSE_PRESS : GF_EVENT_MOUSE_RELEASE;
					_gf_event.data[1] = _x11_event.type == ButtonPress ? true : false;
					gf_pushEvent(win, &_gf_event);
				} break;

				case (KeyPress):
				case (KeyRelease): {
					int32_t	_key;

					_key = XkbKeycodeToKeysym(win->x11.dsp, _x11_event.xkey.keycode, 0, _x11_event.xkey.state & ShiftMask ? 1 : 0);
					_gf_event.data[0] = __gf_getKeycodeFromX11Keysym(_key);
					if (_gf_event.data[0] == GF_KEY_NONE) {
						break;
					}
					_gf_event.type = _x11_event.type == KeyPress ? GF_EVENT_KEY_PRESS : GF_EVENT_KEY_RELEASE;
					_gf_event.data[1] = _x11_event.type == KeyPress ? true : false; 
					gf_pushEvent(win, &_gf_event);
				} break;
			}

		}
	}
	/* As there was no event to be retrieved, we return the 'none' event
	 * */
	memset(event, 0, sizeof(t_event));
	return (false);
}

GFAPI bool	gf_popEvent(t_window win, t_event *e) {
	(void) e;
	if (win->events.cnt <= 0) {
		return (false);
	}
	memset(&win->events.lst[--win->events.cnt], 0, sizeof(t_event));
	return (true);
}

GFAPI bool	gf_pushEvent(t_window win, t_event *e) {
	memcpy(&win->events.lst[win->events.cnt++], e, sizeof(t_event));
	return (true);
}

/* SECTION:
 *  Private interface definitions
 * */

GFAPIS int32_t	__gf_getButtonFromX11Buttons(const int32_t btn) {
	for (size_t i = 0; g_button_map[i].gf != GF_BUTTON_NONE; i++) {
		if (g_button_map[i].x11 == (int32_t) btn) {
			return (g_button_map[i].gf);
		}
	}
	return (GF_KEY_NONE);
}

GFAPIS int32_t	__gf_getKeycodeFromX11Keysym(const int32_t key) {
	for (size_t i = 0; g_keysym_map[i].gf != GF_KEY_NONE; i++) {
		if (g_keysym_map[i].x11 == (int32_t) key) {
			return (g_keysym_map[i].gf);
		}
	}
	return (GF_KEY_NONE);
}
