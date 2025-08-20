#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_egl_h_)
# define _gf_int_egl_h_

# if !defined USE_EGL
#  define USE_EGL
# endif
# include "./../gf.h"
# include <EGL/egl.h>
# include <EGL/eglext.h>

/* SECTION:
 *  Type definitions
 * */

struct s_context {
	EGLDisplay	dsp;
	EGLConfig	conf;
	EGLSurface	surf;
	EGLContext	ctx;
};



/* SECTION:
 *  Internal function declarations
 * */

GFAPII bool	gf_int_safetyCheckEGL(struct s_context *);
GFAPII bool	gf_int_initPlatformEGL(void);




typedef EGLBoolean	(*PFN_eglBindAPI) (EGLenum api);
typedef EGLBoolean	(*PFN_eglBindTexImage) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean	(*PFN_eglChooseConfig) (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLint		(*PFN_eglClientWaitSync) (EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout);
typedef EGLBoolean	(*PFN_eglCopyBuffers) (EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
typedef EGLContext	(*PFN_eglCreateContext) (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
typedef EGLImage	(*PFN_eglCreateImage) (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLAttrib *attrib_list);
typedef EGLSurface	(*PFN_eglCreatePbufferFromClientBuffer) (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
typedef EGLSurface	(*PFN_eglCreatePbufferSurface) (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
typedef EGLSurface	(*PFN_eglCreatePixmapSurface) (EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
typedef EGLSurface	(*PFN_eglCreatePlatformPixmapSurface) (EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLAttrib *attrib_list);
typedef EGLSurface	(*PFN_eglCreatePlatformWindowSurface) (EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list);
typedef EGLSync		(*PFN_eglCreateSync) (EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list);
typedef EGLSurface	(*PFN_eglCreateWindowSurface) (EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
typedef EGLBoolean	(*PFN_eglDestroyContext) (EGLDisplay dpy, EGLContext ctx);
typedef EGLBoolean	(*PFN_eglDestroyImage) (EGLDisplay dpy, EGLImage image);
typedef EGLBoolean	(*PFN_eglDestroySurface) (EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean	(*PFN_eglDestroySync) (EGLDisplay dpy, EGLSync sync);
typedef EGLBoolean	(*PFN_eglGetConfigAttrib) (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
typedef EGLBoolean	(*PFN_eglGetConfigs) (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLContext	(*PFN_eglGetCurrentContext) (void);
typedef EGLDisplay	(*PFN_eglGetCurrentDisplay) (void);
typedef EGLSurface	(*PFN_eglGetCurrentSurface) (EGLint readdraw);
typedef EGLDisplay	(*PFN_eglGetDisplay) (EGLNativeDisplayType display_id);
typedef EGLint		(*PFN_eglGetError) (void);
typedef EGLDisplay	(*PFN_eglGetPlatformDisplay) (EGLenum platform, void *native_display, const EGLAttrib *attrib_list);
typedef void		*(*PFN_eglGetProcAddress) (const char *procname);
typedef EGLBoolean	(*PFN_eglGetSyncAttrib) (EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib *value);
typedef EGLBoolean	(*PFN_eglInitialize) (EGLDisplay dpy, EGLint *major, EGLint *minor);
typedef EGLBoolean	(*PFN_eglMakeCurrent) (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
typedef EGLenum		(*PFN_eglQueryAPI) (void);
typedef EGLBoolean	(*PFN_eglQueryContext) (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
typedef const char	*(*PFN_eglQueryString) (EGLDisplay dpy, EGLint name);
typedef EGLBoolean	(*PFN_eglQuerySurface) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
typedef EGLBoolean	(*PFN_eglReleaseTexImage) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean	(*PFN_eglReleaseThread) (void);
typedef EGLBoolean	(*PFN_eglSurfaceAttrib) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
typedef EGLBoolean	(*PFN_eglSwapBuffers) (EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean	(*PFN_eglSwapInterval) (EGLDisplay dpy, EGLint interval);
typedef EGLBoolean	(*PFN_eglTerminate) (EGLDisplay dpy);
typedef EGLBoolean	(*PFN_eglWaitClient) (void);
typedef EGLBoolean	(*PFN_eglWaitGL) (void);
typedef EGLBoolean	(*PFN_eglWaitNative) (EGLint engine);
typedef EGLBoolean	(*PFN_eglWaitSync) (EGLDisplay dpy, EGLSync sync, EGLint flags);

struct s_EGL {
	void	*handle;
	struct {
		PFN_eglBindAPI							eglBindAPI;
		PFN_eglBindTexImage						eglBindTexImage;
		PFN_eglChooseConfig						eglChooseConfig;
		PFN_eglClientWaitSync					eglClientWaitSync;
		PFN_eglCopyBuffers						eglCopyBuffers;
		PFN_eglCreateContext					eglCreateContext;
		PFN_eglCreateImage						eglCreateImage;
		PFN_eglCreatePbufferFromClientBuffer	eglCreatePbufferFromClientBuffer;
		PFN_eglCreatePbufferSurface				eglCreatePbufferSurface;
		PFN_eglCreatePixmapSurface				eglCreatePixmapSurface;
		PFN_eglCreatePlatformPixmapSurface		eglCreatePlatformPixmapSurface;
		PFN_eglCreatePlatformWindowSurface		eglCreatePlatformWindowSurface;
		PFN_eglCreateSync						eglCreateSync;
		PFN_eglCreateWindowSurface				eglCreateWindowSurface;
		PFN_eglDestroyContext					eglDestroyContext;
		PFN_eglDestroyImage						eglDestroyImage;
		PFN_eglDestroySurface					eglDestroySurface;
		PFN_eglDestroySync						eglDestroySync;
		PFN_eglGetConfigAttrib					eglGetConfigAttrib;
		PFN_eglGetConfigs						eglGetConfigs;
		PFN_eglGetCurrentContext				eglGetCurrentContext;
		PFN_eglGetCurrentDisplay				eglGetCurrentDisplay;
		PFN_eglGetCurrentSurface				eglGetCurrentSurface;
		PFN_eglGetDisplay						eglGetDisplay;
		PFN_eglGetError							eglGetError;
		PFN_eglGetPlatformDisplay				eglGetPlatformDisplay;
		PFN_eglGetProcAddress					eglGetProcAddress;
		PFN_eglGetSyncAttrib					eglGetSyncAttrib;
		PFN_eglInitialize						eglInitialize;
		PFN_eglMakeCurrent						eglMakeCurrent;
		PFN_eglQueryAPI							eglQueryAPI;
		PFN_eglQueryContext						eglQueryContext;
		PFN_eglQueryString						eglQueryString;
		PFN_eglQuerySurface						eglQuerySurface;
		PFN_eglReleaseTexImage					eglReleaseTexImage;
		PFN_eglReleaseThread					eglReleaseThread;
		PFN_eglSurfaceAttrib					eglSurfaceAttrib;
		PFN_eglSwapBuffers						eglSwapBuffers;
		PFN_eglSwapInterval						eglSwapInterval;
		PFN_eglTerminate						eglTerminate;
		PFN_eglWaitClient						eglWaitClient;
		PFN_eglWaitGL							eglWaitGL;
		PFN_eglWaitNative						eglWaitNative;
		PFN_eglWaitSync							eglWaitSync;
	} /* EGL API */;
};
extern struct s_EGL	g_EGL;

#endif
