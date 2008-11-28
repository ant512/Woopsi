#ifndef _LISTBOX_H_
#define _LISTBOX_H_

#include "scrollingpanel.h"
#include "listdata.h"

namespace WoopsiUI {

	/**
	 * Class providing a scrollable list of options.  The ListBox can be set up to only allow one
	 * selection or multiple selections.  Processes double-clicks and raises double-click events,
	 * so that a double-click on an option can be made to automatically select and close a
	 * window/etc.
	 * The options themselves have user-definable text and background colours for their selected
	 * and unselected states.
	 */
	class ListBox : public ScrollingPanel {
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
		 * Add a new option to the gadget using default colours.  Does not redraw the gadget.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 */
		virtual void addOption(const char* text, const u32 value);

		/**
		 * Remove an option from the gadget by its index.  Does not redraw the gadget.
		 * @param index The index of the option to remove.
		 */
		virtual void removeOption(const s32 index);

		/**
		 * Remove all options from the gadget.  Does not redraw the gadget.
		 */
		virtual void removeAllOptions();

		/**
		 * Add a new option to the gadget.  Does not redraw the gadget.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 * @param normalTextColour Colour to draw the text with when not selected.
		 * @param normalBackColour Colour to draw the background with when not selected.
		 * @param selectedTextColour Colour to draw the text with when selected.
		 * @param selectedBackColour Colour to draw the background with when selected.
		 */
		virtual void addOption(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour);

		/**
		 * Select an option by its index.  Does not deselect any other selected options.
		 * Redraws the gadget and raises a value changed event.
		 * @param index The index of the option to select.
		 */
		virtual void selectOption(const s32 index);

		/**
		 * Select an option by its index.  Does not deselect any other selected options.
		 * Redraws the gadget and raises a value changed event.
		 * @param index The index of the option to select.
		 */
		virtual void deselectOption(const s32 index);

		/**
		 * Select all options.  Does nothing if the listbox does not allow multiple selections.
		 * Redraws the gadget and raises a value changed event.
		 */
		virtual void selectAllOptions();

		/**
		 * Deselect all options.
		 * Redraws the gadget and raises a value changed event.
		 */
		virtual void deselectAllOptions();

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
		 * Get the selected index.  Returns -1 if nothing is selected.  If more than one
		 * option is selected, the index of the first selected option is returned.
		 * @return The selected index.
		 */
		virtual const s32 getSelectedIndex() const;

		/**
		 * Sets the selected index.  Specify -1 to select nothing.  Resets any
		 * other selected options to deselected.
		 * Redraws the gadget and raises a value changed event.
		 * @param index The selected index.
		 */
		virtual void setSelectedIndex(const s32 index);

		/**
		 * Get the selected option.  Returns NULL if nothing is selected.
		 * @return The selected option.
		 */
		virtual const ListData::ListDataItem* getSelectedOption() const;
		
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
		
		/**
		 * Sets whether multiple selections are possible or not.
		 * Does not redraw the gadget.
		 * @param allowMultipleSelections True to allow multiple selections.
		 */
		virtual inline void setAllowMultipleSelections(const bool allowMultipleSelections) { _options.setAllowMultipleSelections(allowMultipleSelections); };

		/**
		 * Sets whether multiple selections are possible or not.
		 * Does not redraw the gadget.
		 * @return True if multiple selections are allowed.
		 */
		virtual inline const bool allowsMultipleSelections() const { return _options.allowsMultipleSelections(); };

		/**
		 * Resize the scrolling canvas to encompass all options.
		 * Does not redraw the gadget.
		 */
		virtual void resizeCanvas();

		/**
		 * Get the specified option.
		 * @return The specified option.
		 */
		virtual inline ListData::ListDataItem* getOption(const s32 index) { return _options.getItem(index); };

		/**
		 * Sort the options alphabetically by the text of the options.
		 */
		virtual void sort();

		/**
		 * Get the total number of options.
		 * @return The number of options.
		 */
		virtual inline const s32 getOptionCount() const { return _options.getItemCount(); };

		/**
		 * Get the height of a single option.
		 * @return The height of an option.
		 */
		virtual const u16 getOptionHeight() const;

		/**
		 * Sets whether or not items added to the list are automatically sorted on insert or not.
		 * @param sortInsertedItems True to enable sort on insertion.
		 */
		virtual inline void setSortInsertedItems(const bool sortInsertedItems) { _options.setSortInsertedItems(sortInsertedItems); };

	protected:
		ListData _options;								/**< Option storage. */
		u8 _optionPadding;								/**< Padding between options. */

		/**
		 * Destructor.
		 */
		virtual ~ListBox();

		/**
		 * Select or deselect an option by its index.  Does not deselect any other selected options.
		 * Set index to -1 to select nothing.
		 * Redraws the gadget and raises a value changed event.
		 * @param index The index of the option to select.
		 * @param selected True to select the option, false to deselect it.
		 */
		virtual void setOptionSelected(const s32 index, const bool selected);

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ListBox(const ListBox& listBox) : ScrollingPanel(listBox) { };
	};
}

#endif
