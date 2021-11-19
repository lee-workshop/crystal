#include "numlock.h"
#include <string.h>

NumLock::NumLock() {
}

int NumLock::xkb_init(Display* dpy) {
	int xkb_opcode, xkb_event, xkb_error;
	int xkb_lmaj = XkbMajorVersion;
	int xkb_lmin = XkbMinorVersion;

	return XkbLibraryVersion( &xkb_lmaj, &xkb_lmin )
		&& XkbQueryExtension( dpy, &xkb_opcode, &xkb_event, &xkb_error,
				   &xkb_lmaj, &xkb_lmin );
}

unsigned int NumLock::xkb_mask_modifier( XkbDescPtr xkb, const char *name ) {
	int i;
	if( !xkb || !xkb->names )
		return 0;

	for( i = 0; i < XkbNumVirtualMods; i++ ) {
		char* modStr = XGetAtomName( xkb->dpy, xkb->names->vmods[i] );
		if( modStr != NULL && strcmp(name, modStr) == 0 ) {
			unsigned int mask;
			XkbVirtualModsToReal( xkb, 1 << i, &mask );
			return mask;
		}
	}
	return 0;
}

unsigned int NumLock::xkb_numlock_mask(Display* dpy) {
	XkbDescPtr xkb;

	xkb = XkbGetKeyboard( dpy, XkbAllComponentsMask, XkbUseCoreKbd );
	if( xkb != NULL ) {
		unsigned int mask = xkb_mask_modifier( xkb, "NumLock" );
		XkbFreeKeyboard( xkb, 0, True );
		return mask;
	}
	return 0;
}

void NumLock::control_numlock(Display *dpy, bool flag) {
	unsigned int mask;

	if( !xkb_init(dpy) )
		return;

	mask = xkb_numlock_mask(dpy);
	if( mask == 0 )
		return;

	if( flag == true )
		XkbLockModifiers ( dpy, XkbUseCoreKbd, mask, mask);
	else
		XkbLockModifiers ( dpy, XkbUseCoreKbd, mask, 0);
}

void NumLock::setOn(Display *dpy) {
	control_numlock(dpy, true);
}

void NumLock::setOff(Display *dpy) {
	control_numlock(dpy, false);
}