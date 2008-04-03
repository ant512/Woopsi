#ifndef _WINDOW_BORDER_BUTTON_H_
#define _WINDOW_BORDER_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"

using namespace std;

/**
 * Class representing a button that will appear in the window border.
 */
class WindowBorderButton : public DecorationGlyphButton {

public:
	WindowBorderButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, FontBase* font = NULL);

	/**
	 * Draws the gadget.
	 */
	virtual inline void draw() { Gadget::draw(); };

	virtual void draw(Rect clipRect);

protected:
	/**
	 * Destructor.
	 */
	virtual inline ~WindowBorderButton() { };
};

#endif
