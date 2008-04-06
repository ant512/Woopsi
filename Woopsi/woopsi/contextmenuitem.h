#ifndef _CONTEXT_MENU_ITEM_H_
#define _CONTEXT_MENU_ITEM_H_

#include "textbox.h"

using namespace std;

class ContextMenuItem : public Textbox {
public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the menu item, relative to its parent.
	 * @param y The y co-ordinate of the menu item, relative to its parent.
	 * @param width The width of the menu item.
	 * @param height The height of the menu item.
	 * @param text Pointer to a string to display in the menu item.
	 * @param font The font to use in this menu item.
	 */
	ContextMenuItem(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font = NULL);

	/**
	 * Draw the region of the menu item within the clipping rect.
	 * @param clipRect The clipping rect to limit drawing to.
	 */
	virtual void draw(Rect clipRect);
	
	/**
	 * Draw all visible regions of the menu.
	 */
	virtual inline void draw() { Gadget::draw(); };

private:

	/**
	 * Destructor.
	 */
	virtual inline ~ContextMenuItem() { };
};

#endif
