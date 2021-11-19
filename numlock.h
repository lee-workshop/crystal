#ifndef _NUMLOCK_H_
#define _NUMLOCK_H_

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

class NumLock {

public:
	NumLock();
	static void setOn(Display *dpy);
	static void setOff(Display *dpy);

private:
	static int xkb_init(Display *dpy);
	static unsigned int xkb_mask_modifier(XkbDescPtr xkb, const char *name);
	static unsigned int xkb_numlock_mask(Display *dpy);
	static void control_numlock(Display *dpy, bool flag);
};

#endif /* _NUMLOCK_H_ */
