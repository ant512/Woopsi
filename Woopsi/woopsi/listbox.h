#ifndef _LISTBOX_H_
#define _LISTBOX_H_

#include "scrollingpanel.h"

using namespace std;

/**
 */
class ListBox : public ScrollingPanel {
public:
	
	/**
	 * Struct defining a single listbox option.
	 */
	typedef struct {
		char* text;							/**< Text to display for option. */
		u32 value;							/**< Option value. */
		u16 normalTextColour;				/**< Colour used for text when not selected */
		u16 normalBackColour;				/**< Colour used for background when not selected */
		u16 selectedTextColour;				/**< Colour used for text when selected */
		u16 selectedBackColour;				/**< Colour used for background when selected */
	} ListBoxOption;
	
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
	 * Add a new option to the gadget using default colours.
	 * @param text Text to show in the option.
	 * @param value The value of the option.
	 */
	virtual void addOption(const char* text, const u32 value);

	/**
	 * Add a new option to the gadget.
	 * @param text Text to show in the option.
	 * @param value The value of the option.
	 * @param normalTextColour Colour to draw the text with when not selected.
	 * @param normalBackColour Colour to draw the background with when not selected.
	 * @param selectedTextColour Colour to draw the text with when selected.
	 * @param selectedBackColour Colour to draw the background with when selected.
	 */
	virtual void addOption(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour);

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
	inline const s32 getSelectedIndex() const { return _selectedIndex; };

	/**
	 * Sets the selected index.  Specify -1 to select nothing.
	 * @param The selected index.
	 */
	void setSelectedIndex(const s32 index);

	/**
	 * Get the selected index.  Returns -1 if nothing is selected.
	 * @return The selected index.
	 */
	inline const ListBoxOption* getSelectedOption() const { return _options[_selectedIndex]; };
	
	/**
	 * Click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y);

	/**
	 * Double-click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool doubleClick(s16 x, s16 y);

private:
	DynamicArray<ListBoxOption*> _options;			/**< Array of options. */
	s32 _selectedIndex;								/**< Index of the currently selected option. */
	u8 _optionPadding;								/**< Padding between options. */

	/**
	 * Destructor.
	 */
	virtual inline ~ListBox() { };
};

#endif
