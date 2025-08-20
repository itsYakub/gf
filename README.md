<div align="center">
  
<img src="./res/img/gf-logo-red.png">

# GF - Graphics Framework

</div>

<div align="center">
  
![GitHub commit activity](https://img.shields.io/github/commit-activity/t/itsYakub/gf?style=for-the-badge)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/itsYakub/gf/master?style=for-the-badge)
![Static Badge](https://img.shields.io/badge/Made_with-C99-blue?style=for-the-badge)
![GitHub License](https://img.shields.io/github/license/itsYakub/gf?style=for-the-badge)

</div>

## Introduction:

```c
#include "gf.h"

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

```

This is **gf** - graphics framework for GNU/Linux and Windows OpenGL programming.
It's incredibly tiny, simple yet powerful codebase allows you to not only develop efficient native graphical application,
but also to sharpen your knowledge with low - level windowing and graphics, thanks to compact and commented code!

The main philosophy of this project is quite simple: *I wanted to learn how to make decent windowing library, focusing on speed and simplicity.* <br>
**gf** borrows concept from most popular and battle-tested libraries out there: SDL3, glfw, RGFW, raylib to name a few.

I hope you'll have lot's of fun using **gf**!

*NOTE:* <br>
*For the current state of the project, **gf** only supports **GNU/LINUX** operating system, with Xorg and Wayland windowing servers. In the future I plan to add support for Win32 API and potentialy MacOS support.*

## Building:
<details>
<summary><h3>GNU/Linux:</h3></summary>

*NOTE:* <br>
*This building steps are described for Debian-based distributions of GNU/Linux. If you're using something different, search for different packages in your package manager.*

#### 1. Download dependencies:

For X11:
- ensure that either: **libX11.so**, **libX11.so.6**, **libX11.so.6.4.0** is present on your system and in your **$PATH**
- ensure that either: **libEGL.so**, **libEGL.so.1** is present on your system and in your **$PATH**
- ensure that either: **libGLX.so**, **libGLX.so.1** is present on your system and in your **$PATH**
- ensure that either: **libGL.so**, **libGL.so.1** is present on your system and in your **$PATH**

#### 2. Clone this repository:

```console
$ git clone https://github.com/itsYakub/gf.git
$ cd ./gf/
```
#### 3. Build the project using GNU Make:

```console
$ make all
```

#### 4. Run one of demo program supplied with the repository:

For the X11 support:
```console
$ gcc ./demo/00-hello-world.c -L. -lgf -lGL
```

</details>
<details>
<summary><h3>Windows:</h3></summary>
  
*NOTE:* <br>
*As of now, **gf** doesn't support Windows platform.*

</details>
<details>
<summary><h3>MacOS:</h3></summary>

*NOTE:* <br>
*As of now, **gf** doesn't support MacOS platform.*

</details>

## Licence:

This project is under the [LGPL-3.0 license](./LICENCE).
