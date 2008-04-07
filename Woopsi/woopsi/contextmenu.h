#ifndef _CONTEXT_MENU_H_
#define _CONTEXT_MENU_H_

#include "gadget.h"
#include "eventhandler.h"

using namespace std;

class ContextMenuItem;

class ContextMenu : public Gadget, public EventHandler {
public:

	/**
	 * Constructor.
	 * @
	 */
	ContextMenu(FontBase* font = NULL);

	/**
	 * Handles events raised by the gadget's sub-gadgets.
	 */
	virtual bool handleEvent(const EventArgs& e);

	/**
	 * Add a new menu item to the gadget.
	 * @param text Text to show in the item.
	 */
	virtual ContextMenuItem* newMenuItem(char* text, u32 value);

	/**
	 * Draw the region of the menu within the clipping rect.
	 * @param clipRect The clipping rect to limit drawing to.
	 */
	virtual void draw(Rect clipRect);
	
	/**
	 * Draw all visible regions of the menu.
	 */
	virtual inline void draw() { Gadget::draw(); };

	/**
	 * Resize the gadget to the new dimensions.
	 * @param width The new width.
	 * @param height The new height.
	 * @return True if the resize was successful.
	 */
	virtual bool resize(u16 width, u16 height);

	/**
	 * Clear all items from the menu and reset it.
	 */
	virtual void reset();

	/**
	 * Set the gadget that opened the menu.
	 * @param gadget Pointer to the gadget that opened the menu.
	 */
	inline void setOpener(Gadget* gadget) { _opener = gadget; };

	/**
	 * Get the gadget that opened the menu.
	 * @return Pointer to the gadget that opened the menu.
	 */
	inline Gadget* getOpener() { return _opener; };

private:
	Gadget* _opener;

	/**
	 * Destructor.
	 */
	virtual inline ~ContextMenu() { };
};

#endif
