#if !defined USE_X11
# define USE_X11
#endif
#include "./../gf-int.h"
#include "./../gf.h"

/* SECTION:
 *  Public API implementation
 * */

/* TODO(yakub):
 *  Implement those
 * */

GFAPI bool	gf_clipCopy(t_window win, const char *data, size_t size) {
	gf_int_safetyCheckX11(&win->client);

	if (!data) { return (false); } 

	/* Copy the function parameters to internal clipboard data
	 * */
	if (win->clipboard.data) {
		g_X11.XFree(win->clipboard.data), win->clipboard.data = 0;
	}
	win->clipboard.data = calloc(size + 1, sizeof(char));
	win->clipboard.data = memcpy(win->clipboard.data, data, size);
	win->clipboard.size = size;

	/* Perform a clipboard operation
	 * */
	g_X11.XSetSelectionOwner(win->client.dsp, win->client.atoms.CLIPBOARD, win->client.id, 0);
	if (g_X11.XGetSelectionOwner(win->client.dsp, win->client.atoms.CLIPBOARD) != win->client.id) { return (false); }

	g_X11.XSync(win->client.dsp, 0);
	return (gf_int_pollInternalEvents(win));
}

GFAPI bool	gf_clipPaste(t_window win, char **datptr, size_t *sizptr) {
	gf_int_safetyCheckX11(&win->client);

	/* Perform a clipboard operation
	 * */
	g_X11.XConvertSelection(
		win->client.dsp,
		win->client.atoms.CLIPBOARD,
		win->client.atoms.UTF8_STRING,
		win->client.atoms.XSEL_DATA,
		win->client.id,
		CurrentTime
	);

	g_X11.XSync(win->client.dsp, 0);
	gf_int_pollInternalEvents(win);

	/* Copy the clipboard data to the function return parameters
	 * */
	if (datptr) {
		*datptr = calloc(strlen(win->clipboard.data) + 1, sizeof(char));
		*datptr = memcpy(*datptr, win->clipboard.data, strlen(win->clipboard.data));
	}
	if (sizptr) {
		*sizptr = win->clipboard.size;
	}
	return (true);
}
