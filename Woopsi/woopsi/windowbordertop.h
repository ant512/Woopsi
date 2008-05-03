#ifndef _WINDOW_BORDER_TOP_H_
#define _WINDOW_BORDER_TOP_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

/**
 * Class providing a gadget that mimics the appearance of the AmigaOS 3.x
 * window title bar.  Forms part of the AmigaWindow gadget.
 */
class WindowBorderTop : public Gadget {

public:
	/**
	 * Constructor.
	 * @param x The x co-ordinate of the gadget.
	 * @param width The width of the gadget.
	 * @param height The height of the gadget.
	 * @param text The text to display in the title bar.
	 * @param font The font to display the text with.
	 */
	WindowBorderTop(s16 x, u16 width, u16 height, const char* text, FontBase* font = NULL);

	/**
	 * Draws the gadget.
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
	 * Give the gadget focus.
	 * @return True if the gadget received focus correctly.
	 */
	virtual bool focus();

	/**
	 * Remove focus from the gadget.
	 * @return True if the gadget lost focus correctly.
	 */
	virtual bool blur();

	/**
	 * Click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y);

protected:
	const char* _text;								/**< Text to display in the gadget */

	/**
	 * Destructor.
	 */
	virtual inline ~WindowBorderTop() {	};
};

#endif
