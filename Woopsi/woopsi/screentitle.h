#ifndef _SCREEN_TITLE_H_
#define _SCREEN_TITLE_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

/**
 * Title bar for the AmigaScreen class.  Mimics the appearance of
 * the title bar from Amiga Workbench 3.x.  Shows the screen's name
 * and offers depth (z-order) and flip (physical display swapping)
 * buttons.
 */
class ScreenTitle : public Gadget {

public:

	/**
	 * Constructor.
	 * @param height The height of the title bar.
	 * @param text The text to display in the title bar.
	 * @param font The font to use for the title bar.
	 */
	ScreenTitle(u16 height, char* text, FontBase* font = NULL);

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

	/**
	 * Click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y);

protected:
	char* _text;							/**< Text to display in the title bar */

	/**
	 * Destructor.
	 */
	virtual inline ~ScreenTitle() {
		if (_text != NULL) {
			delete [] _text;
		}
	}
};

#endif
