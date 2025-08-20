#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_glx_h_)
# define _gf_int_glx_h_

# if !defined USE_GLX
#  define USE_GLX
# endif
# include "./../gf.h"
# include <GL/glx.h>
# include <GL/glxext.h>

/* SECTION:
 *  Type definitions
 * */

struct s_context {
	GLXFBConfig	conf;
	GLXContext	id;
};



/* SECTION:
 *  Internal function declarations
 * */

GFAPII bool	gf_int_safetyCheckGLX(struct s_context *);
GFAPII bool	gf_int_initPlatformGLX(void);



/* SECTION:
 *  Platform
 * */

typedef Bool		(*PFN_glXQueryExtension) (Display *, int32_t *, int32_t *);
typedef Bool		(*PFN_glXQueryVersion) (Display *, int32_t *, int32_t *);
typedef const char	*(*PFN_glXQueryExtensionsString) (Display *, int32_t);
typedef const char	*(*PFN_glXGetClientString) (Display *, int32_t);
typedef const char	*(*PFN_glXQueryServerString) (Display *, int32_t, int32_t);
typedef GLXFBConfig	*(*PFN_glXGetFBConfigs) (Display *, int32_t, int32_t *);
typedef GLXFBConfig	*(*PFN_glXChooseFBConfig) (Display *, int32_t, const int32_t *, int32_t *);
typedef int32_t		(*PFN_glXGetFBConfigAttrib) (Display *, GLXFBConfig, int32_t, int32_t *);
typedef XVisualInfo	*(*PFN_glXGetVisualFromFBConfig) (Display *, GLXFBConfig);
typedef GLXWindow	(*PFN_glXCreateWindow) (Display *, GLXFBConfig, Window, const int32_t *);
typedef void		(*PFN_glXDestroyWindow) (Display *, GLXWindow);
typedef GLXPixmap	(*PFN_glXCreatePixmap) (Display *, GLXFBConfig, Pixmap, const int32_t *);
typedef void		(*PFN_glXDestroyPixmap) (Display *, GLXPixmap);
typedef GLXPbuffer	(*PFN_glXCreatePbuffer) (Display *, GLXFBConfig, const int32_t *);
typedef void		(*PFN_glXDestroyPbuffer) (Display *, GLXPbuffer);
typedef void		(*PFN_glXQueryDrawable) (Display *, GLXDrawable, int32_t, uint32_t *);
typedef GLXContext	(*PFN_glXCreateNewContext) (Display *, GLXFBConfig, int32_t, GLXContext, Bool);
typedef Bool		(*PFN_glXIsDirect) (Display *, GLXContext);
typedef void		(*PFN_glXDestroyContext) (Display *, GLXContext);
typedef Bool		(*PFN_glXMakeContextCurrent) (Display *, GLXDrawable, GLXDrawable, GLXContext);
typedef void		(*PFN_glXCopyContext) (Display *, GLXContext, GLXContext, uint64_t);
typedef GLXContext	(*PFN_glXGetCurrentContext) (void);
typedef GLXDrawable	(*PFN_glXGetCurrentDrawable) (void);
typedef GLXDrawable	(*PFN_glXGetCurrentReadDrawable) (void);
typedef Display *	(*PFN_glXGetCurrentDisplay) (void);
typedef int32_t		(*PFN_glXQueryContext) (Display *, GLXContext, int32_t, int32_t *);
typedef void		(*PFN_glXSelectEvent) (Display *, GLXDrawable, uint64_t);
typedef void		(*PFN_glXGetSelectedEvent) (Display *, GLXDrawable, uint64_t *);
typedef void		(*PFN_glXWaitGL) (void);
typedef void		(*PFN_glXWaitX) (void);
typedef void		(*PFN_glXSwapBuffers) (Display *, GLXDrawable);
typedef void		(*PFN_glXUseXFont) (Font, int32_t, int32_t, int32_t);
typedef void		(*PFN_glXGetProcAddress) (const GLubyte *);
typedef int32_t		(*PFN_glXGetConfig) (Display *, XVisualInfo *, int32_t, int32_t *);
typedef XVisualInfo *(*PFN_glXChooseVisual) (Display *, int32_t, int32_t *);
typedef GLXPixmap	(*PFN_glXCreateGLXPixmap) (Display *, XVisualInfo *, Pixmap);
typedef void		(*PFN_glXDestroyGLXPixmap) (Display *, GLXPixmap);
typedef GLXContext	(*PFN_glXCreateContext) (Display *, XVisualInfo *, GLXContext, Bool);
typedef Bool		(*PFN_glXMakeCurrent) (Display *, GLXDrawable, GLXContext);
typedef GLXContext	(*PFN_glXCreateContextAttribsARB) (Display *, GLXFBConfig, GLXContext, Bool, const int32_t *);

struct s_GLX {
	void	*handle;
	struct {
		PFN_glXQueryExtension			glXQueryExtension;
		PFN_glXQueryVersion				glXQueryVersion;
		PFN_glXQueryExtensionsString	glXQueryExtensionsString;
		PFN_glXGetClientString			glXGetClientString;
		PFN_glXQueryServerString		glXQueryServerString;
		PFN_glXGetFBConfigs				glXGetFBConfigs;
		PFN_glXChooseFBConfig			glXChooseFBConfig;
		PFN_glXGetFBConfigAttrib		glXGetFBConfigAttrib;
		PFN_glXGetVisualFromFBConfig	glXGetVisualFromFBConfig;
		PFN_glXCreateWindow				glXCreateWindow;
		PFN_glXDestroyWindow			glXDestroyWindow;
		PFN_glXCreatePixmap				glXCreatePixmap;
		PFN_glXDestroyPixmap			glXDestroyPixmap;
		PFN_glXCreatePbuffer			glXCreatePbuffer;
		PFN_glXDestroyPbuffer			glXDestroyPbuffer;
		PFN_glXQueryDrawable			glXQueryDrawable;
		PFN_glXCreateNewContext			glXCreateNewContext;
		PFN_glXIsDirect					glXIsDirect;
		PFN_glXDestroyContext			glXDestroyContext;
		PFN_glXMakeContextCurrent		glXMakeContextCurrent;
		PFN_glXCopyContext				glXCopyContext;
		PFN_glXGetCurrentContext		glXGetCurrentContext;
		PFN_glXGetCurrentDrawable		glXGetCurrentDrawable;
		PFN_glXGetCurrentReadDrawable	glXGetCurrentReadDrawable;
		PFN_glXGetCurrentDisplay		glXGetCurrentDisplay;
		PFN_glXQueryContext				glXQueryContext;
		PFN_glXSelectEvent				glXSelectEvent;
		PFN_glXGetSelectedEvent			glXGetSelectedEvent;
		PFN_glXWaitGL					glXWaitGL;
		PFN_glXWaitX					glXWaitX;
		PFN_glXSwapBuffers				glXSwapBuffers;
		PFN_glXUseXFont					glXUseXFont;
		PFN_glXGetProcAddress			glXGetProcAddress;
		PFN_glXGetConfig				glXGetConfig;
		PFN_glXChooseVisual				glXChooseVisual;
		PFN_glXCreateGLXPixmap			glXCreateGLXPixmap;
		PFN_glXDestroyGLXPixmap			glXDestroyGLXPixmap;
		PFN_glXCreateContext			glXCreateContext;
		PFN_glXMakeCurrent				glXMakeCurrent;
		PFN_glXCreateContextAttribsARB	glXCreateContextAttribsARB;
	} /* GLX API */;
};

extern struct s_GLX	g_GLX;

#endif
