#include "./../src/gf.h"

#include <GL/gl.h>

int	main(void) {
	t_window	window;

	gf_createWindow(&window, 960, 540, "gf 1.0 - Hello, World!");
	gf_makeCurrent(window);
	for ( ;; ) {
		t_event		event;
	
		while (gf_pollEvents(window, &event)) {
			switch (event.type) {

				case (GF_EVENT_QUIT): {
					gf_logi("EVENT: QUIT\n");

					gf_destroyContext(window);
					gf_destroyWindow(window);
					return (0);
				}

				case (GF_EVENT_MINIMIZE): {
					gf_logi("EVENT: MINIMIZE\n");
				} break;
				case (GF_EVENT_MAXIMIZE): {
					gf_logi("EVENT: MAXIMIZE\n");
				} break;
				case (GF_EVENT_FULLSCREEN): {
					gf_logi("EVENT: FULLSCREEN\n");
				} break;
				
				case (GF_EVENT_RESIZE): {
					gf_logi("EVENT: RESIZE (w.:%d, h.:%d)\n", event.resize.w, event.resize.h);
				} break;
				case (GF_EVENT_MOVE): {
					gf_logi("EVENT: MOVE (x.:%d, y.:%d)\n", event.move.x, event.move.y);
				} break;

				case (GF_EVENT_MOUSEMOTION): {
					gf_logi("EVENT: MOUSEMOTION (w.:%d, h.:%d)\n", event.motion.x, event.motion.y);
				} break;
				case (GF_EVENT_MOUSE_FOCUS): {
					gf_logi("EVENT: MOUSE FOCUS (state:%d)\n", event.focus.state);
				} break;

				case (GF_EVENT_MOUSE_SCROLL): {
					gf_logi("EVENT: MOUSE SCROLL (value:%d)\n", event.scroll.val);
				} break;
				case (GF_EVENT_MOUSE_PRESS): {
					gf_logi("EVENT: MOUSE PRESS (btn.:%d, state:%d)\n", event.button.btn, event.button.state);
				} break;
				case (GF_EVENT_MOUSE_RELEASE): {
					gf_logi("EVENT: MOUSE RELEASE (btn.:%d, state:%d)\n", event.button.btn, event.button.state);
				} break;

				case (GF_EVENT_KEY_PRESS): {
					gf_logi("EVENT: KEY PRESS (key.:%s, state:%d)\n", gf_keyToString(event.key.key), event.key.state);

					if (event.key.key == GF_KEY_V) {
						char	*c;
						size_t	s;
						gf_clipPaste(window, &c, &s);
					}
				} break;
				case (GF_EVENT_KEY_RELEASE): {
					gf_logi("EVENT: KEY RELEASE (key.:%s, state:%d)\n", gf_keyToString(event.key.key), event.key.state);
				} break;

				case (GF_EVENT_CLIPBOARD): {
					gf_logi("EVENT: CLIPBOARD (data.:%s, size:%lu)\n", event.clip.data, event.clip.size);
				} break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		gf_swapBuffers(window);
	
	}

	gf_destroyContext(window);	
	gf_destroyWindow(window);
	
	return (0);
}
