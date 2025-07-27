#include "./../gf.h"
#include "./gf-int.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

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
						_gf_event.s_data.i[0] = win->misc.width = _x11_event.xconfigure.width;
						_gf_event.s_data.i[1] = win->misc.height = _x11_event.xconfigure.height;
					}
					else if (
						win->misc.x != _x11_event.xconfigure.x ||
						win->misc.y != _x11_event.xconfigure.y
					) {
						_gf_event.type = GF_EVENT_MOVE;
						_gf_event.s_data.i[0] = win->misc.x = _x11_event.xconfigure.x;
						_gf_event.s_data.i[1] = win->misc.y = _x11_event.xconfigure.y;
					}
					else { break; }
					gf_pushEvent(win, &_gf_event);
				} break;	

				case (MotionNotify): {
					_gf_event.type = GF_EVENT_MOUSEMOTION;
					_gf_event.s_data.i[0] = _x11_event.xmotion.x;
					_gf_event.s_data.i[1] = _x11_event.xmotion.y;
					_gf_event.s_data.i[2] = _x11_event.xmotion.x_root;
					_gf_event.s_data.i[3] = _x11_event.xmotion.y_root;
					gf_pushEvent(win, &_gf_event);
				} break;
				/* NOTE(yakub):
				 *  Under the hood, x11 maps the mouse buttons to:
				 *  - [1] -> LEFT;
				 *  - [2] -> MIDDLE;
				 *  - [3] -> RIGHT;
				 * */
				case (ButtonPress): {
					_gf_event.type = GF_EVENT_MOUSE_PRESS;
					if (_x11_event.xbutton.button == 1) { _gf_event.s_data.c[0] = GF_BUTTON_LEFT; }
					else if (_x11_event.xbutton.button == 2) { _gf_event.s_data.c[0] = GF_BUTTON_MIDDLE; }
					else if (_x11_event.xbutton.button == 3) { _gf_event.s_data.c[0] = GF_BUTTON_RIGHT; }
					else { break; }
					_gf_event.s_data.c[1] = true;
					gf_pushEvent(win, &_gf_event);
				} break;
				case (ButtonRelease): {
					_gf_event.type = GF_EVENT_MOUSE_RELEASE;
					if (_x11_event.xbutton.button == 1) { _gf_event.s_data.c[0] = GF_BUTTON_LEFT; }
					else if (_x11_event.xbutton.button == 2) { _gf_event.s_data.c[0] = GF_BUTTON_MIDDLE; }
					else if (_x11_event.xbutton.button == 3) { _gf_event.s_data.c[0] = GF_BUTTON_RIGHT; }
					else { break; }
					_gf_event.s_data.c[1] = false;
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
