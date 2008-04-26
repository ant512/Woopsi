#ifndef _LISTBOX_ITEM_H_
#define _LISTBOX_ITEM_H_

#include "textbox.h"

using namespace std;

/**
 * Class representing an item in the ListBox class.
 */
class ListBoxItem : public Textbox {
public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the listbox item, relative to its parent.
	 * @param y The y co-ordinate of the listbox item, relative to its parent.
	 * @param width The width of the listbox item.
	 * @param height The height of the listbox item.
	 * @param text Pointer to a string to display in the menu item.
	 * @param value The value of the item.
	 * @param normalTextColour Colour for text output when unselected.
	 * @param normalBackColour Colour for background when unselected.
	 * @param selectedTextColour Colour for text output when selected.
	 * @param selectedBackColour Colour for background when selected.
	 * @param font The font to use in this menu item.
	 */
	ListBoxItem(s16 x, s16 y, u16 width, u16 height, const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour, FontBase* font = NULL);

	/**
	 * Draw the region of the listbox item within the clipping rect.
	 * @param clipRect The clipping rect to limit drawing to.
	 */
	virtual void draw(Rect clipRect);

	/**
	 * Draw all visible regions of the listbox item.
	 */
	virtual inline void draw() { Gadget::draw(); };

	/**
	 * Get the listbox item's value.
	 * @return The listbox item's value.
	 */
	inline u32 getValue() { return _value; };

	/**
	 * Is this item selected?
	 */
	inline const bool isSelected() const { return _selected; };

	/**
	 * Select this item.
	 * @return True if the item was selected successfully.
	 */
	bool select();

	/**
	 * Unselect this item.
	 * @return True if the item was unselected successfully.
	 */
	bool unselect();

private:
	u32 _value;								/**< The value of the menu item */
	u16 _normalTextColour;
	u16 _normalBackColour;
	u16 _selectedTextColour;
	u16 _selectedBackColour;
	bool _selected;

	/**
	 * Destructor.
	 */
	virtual inline ~ListBoxItem() { };
};

#endif
