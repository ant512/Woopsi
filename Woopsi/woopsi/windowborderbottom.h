#ifndef _WINDOW_BORDER_BOTTOM_H_
#define _WINDOW_BORDER_BOTTOM_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

class WindowBorderBottom : public Gadget {

public:
	WindowBorderBottom(s16 x, s16 y, u16 width, u16 height, u8 sideBorderSize);

	/**
	 * Draws the gadget.
	 */
	virtual inline void draw() { Gadget::draw(); };

	virtual void draw(Rect clipRect);

	virtual bool focus();
	virtual bool blur();

protected:
	u8 _sideBorderSize;

	/**
	 * Destructor.
	 */
	virtual inline ~WindowBorderBottom() { };
};

#endif
