#if defined (__cplusplus)
# pragma once
#endif
#if !defined (_gf_int_x11_h_)
# define _gf_int_x11_h_

# if !defined USE_X11
#  define USE_X11
# endif
# include "./../gf-int.h"
# include "./../gf.h"

/* SECTION:
 *  Event Interface
 * */

GFAPII bool		gf_int_pollInternal_Client(t_window, XEvent *);
GFAPII bool		gf_int_pollInternal_Property(t_window, XEvent *);
GFAPII bool		gf_int_pollInternal_Configure(t_window, XEvent *);
GFAPII bool		gf_int_pollInternal_Mouse(t_window, XEvent *);
GFAPII bool		gf_int_pollInternal_Key(t_window, XEvent *);

#endif
