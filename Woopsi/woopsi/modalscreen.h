#ifndef _MODAL_SCREEN_H_
#define _MODAL_SCREEN_H_

#include <nds.h>
#include "screen.h"

using namespace std;

/**
 * Invisible screen class.  Should be used whenever a modal window is needed, as it will block
 * out all interaction with any gadgets but its own children.
 */
class ModalScreen : public Screen {

public:
	/**
	 * Constructor.
	 * @param title The title of the screen; not displayed by default.
	 * @param font The font to use with the screen.
	 */
	ModalScreen(char* title, FontBase* font = NULL);
	
	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	/**
	 * Draw the area of this gadget that falls within the clipping region.
	 * Called by the draw() function to draw all visible regions.
	 * @param clipRect The clipping region to draw.
	 * @see draw()
	 */
	virtual void draw(Rect clipRect);

protected:

	/**
	 * Destructor.
	 */
	virtual inline ~ModalScreen() { };
};

#endif
