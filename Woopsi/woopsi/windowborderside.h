#ifndef _WINDOW_BORDER_SIDE_H_
#define _WINDOW_BORDER_SIDE_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

class WindowBorderSide : public Gadget {
public:
	WindowBorderSide(s16 x, s16 y, u16 width, u16 height);

	virtual void draw(Rect clipRect);
	virtual void draw();

	virtual bool focus();
	virtual bool blur();

protected:
	/**
	 * Destructor.
	 */
	virtual inline ~WindowBorderSide() { };
};

#endif
