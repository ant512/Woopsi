#ifndef _LISTBOX_H_
#define _LISTBOX_H_

#include "gadget.h"
#include "eventhandler.h"

using namespace std;

class ListBoxItem;

/**
 */
class ListBox : public Gadget, public EventHandler {
public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the gadget.
	 * @param y The y co-ordinate of the gadget.
	 * @param width The width of the gadget.
	 * @param height The height of the gadget.
	 * @param font The font to use with the listbox.
	 */
	ListBox(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

	/**
	 * Handles events raised by the gadget's sub-gadgets.
	 * @param e The event arguments.
	 */
	virtual bool handleEvent(const EventArgs& e);

	/**
	 * Add a new menu item to the gadget using default colours.
	 * @param text Text to show in the item.
	 * @param value The value of the item.
	 * @return A pointer to the new item.
	 */
	virtual ListBoxItem* newListBoxItem(const char* text, const u32 value);

	/**
	 * Add a new menu item to the gadget.
	 * @param text Text to show in the item.
	 * @param value The value of the item.
	 * @param normalTextColour Colour to draw the text with when not selected.
	 * @param normalBackColour Colour to draw the background with when not selected.
	 * @param selectedTextColour Colour to draw the text with when selected.
	 * @param selectedBackColour Colour to draw the background with when selected.
	 */
	virtual ListBoxItem* newListBoxItem(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour);

	/**
	 * Draw the region of the menu within the clipping rect.
	 * @param clipRect The clipping rect to limit drawing to.
	 */
	virtual void draw(Rect clipRect);

	/**
	 * Draw all visible regions of the gadget.
	 */
	virtual inline void draw() { Gadget::draw(); };

	/**
	 * Get the selected index.  Returns -1 if nothing is selected.
	 * @return The selected index.
	 */
	inline const s16 getSelectedIndex() const { return getGadgetIndex(_selectedGadget); };

	/**
	 * Sets the selected index.  Specify -1 to select nothing.
	 * @param The selected index.
	 */
	void setSelectedIndex(const s32 index);

	/**
	 * Get the selected index.  Returns -1 if nothing is selected.
	 * @return The selected index.
	 */
	inline const ListBoxItem* getSelectedItem() const { return (ListBoxItem*)_selectedGadget; };

private:
	Gadget* _selectedGadget;						/**< The selected gadget */

	/**
	 * Destructor.
	 */
	virtual inline ~ListBox() { };

	/**
	 * Set the selected gadget.
	 * @param gadget The gadget to select.
	 */
	void setSelectedGadget(Gadget* gadget);
};

#endif
