#include "./../src/gf.h"

#include <stdio.h>

#include <GL/gl.h>

int	main(void) {
	t_window	window;
	t_event		event;

	int32_t		width;
	int32_t		height;
	bool		exit;

	if (!gf_createWindow(&window, 960, 540, "gf 1.0 - Hello, World!", 0)) {
		return (1);
	}

	exit = false;
	while (!exit) {
	
		while (gf_pollEvents(window, &event)) {
			switch (event.type) {

				case (GF_EVENT_QUIT): {
					printf("Event: QUIT\n");
					exit = true;
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
