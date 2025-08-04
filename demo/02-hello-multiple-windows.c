#include "./../src/gf.h"

#include <GL/gl.h>

int	main(void) {
	t_window	window0;
	t_window	window1;

	gf_createWindow(&window0, 960, 540, "gf 1.0 - Hello, World!");
	gf_createWindow(&window1, 960, 540, "gf 1.0 - Hello, World!");

	for ( ;; ) {
		t_event		event;
	
		while (gf_pollEvents(window0, &event)) {
			if (event.type == GF_EVENT_QUIT) { goto exit; }
		}
	
		while (gf_pollEvents(window1, &event)) {
			if (event.type == GF_EVENT_QUIT) { goto exit; }
		}

		gf_makeCurrent(window0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.8f, 0.1f, 0.1f, 1.0f);
		gf_swapBuffers(window0);

		gf_makeCurrent(window1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.8f, 0.1f, 1.0f);
		gf_swapBuffers(window1);
	}
	
exit:

	gf_destroyContext(window0); gf_destroyWindow(window0);
	gf_destroyContext(window1); gf_destroyWindow(window1);
	
	return (0);
}
