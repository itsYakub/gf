#include "./../src/gf.h"

#include <GL/gl.h>

int	main(void) {
	t_window	window;

	if (!gf_createWindow(&window, 960, 540, "gf 1.0 - Hello, World!", GF_WINDOW_RESIZABLE | GF_WINDOW_VSYNC_HINT)) {
		return (1);
	}

	for ( ;; ) {
		t_event		event;
	
		while (gf_pollEvents(window, &event)) {
			switch (event.type) {

				case (GF_EVENT_QUIT): {
					gf_logi("EVENT: QUIT\n");

					gf_destroyWindow(window);
					return (0);
				} break;
				
				case (GF_EVENT_RESIZE): {
					gf_logi("EVENT: RESIZE\n");
				} break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		gf_swapBuffers(window);
	
	}
	
	gf_destroyWindow(window);
	
	return (0);
}
