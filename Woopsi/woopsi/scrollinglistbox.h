#ifndef _SCROLLING_LISTBOX_H_
#define _SCROLLING_LISTBOX_H_

#include "gadget.h"
#include "listbox.h"
#include "eventhandler.h"
#include "listdata.h"

namespace WoopsiUI {

	class ScrollbarVertical;

	/**
	 * Gadget containing a ListBox and a vertical scrollbar.  Exposed
	 * methods are more or less identical to the methods exposed by the ListBox
	 * to ensure that the two are interchangeable.
	 */
	class ScrollingListBox : public Gadget, public EventHandler {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param font The font to use with the listbox.
		 */
		ScrollingListBox(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

		/**
		 * Add a new option to the gadget using default colours.  Does not redraw the gadget.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 */
		virtual void addOption(const char* text, const u32 value);

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
		 * Remove an option from the gadget by its index.  Does not redraw the gadget.
		 * @param index The index of the option to remove.
		 */
		virtual void removeOption(const s32 index);

		/**
		 * Remove all options from the gadget.  Does not redraw the gadget.
		 */
		virtual void removeAllOptions();

		/**
		 * Select an option by its index.  Does not deselect any other selected options.
		 * Redraws the gadget and raises a value changed event.
		 * @param index The index of the option to select.
		 */
		virtual inline void selectOption(const s32 index) {
			_listbox->selectOption(index);
		};

		/**
		 * Select an option by its index.  Does not deselect any other selected options.
		 * Redraws the gadget and raises a value changed event.
		 * @param index The index of the option to select.
		 */
		virtual inline void deselectOption(const s32 index) {
			_listbox->deselectOption(index);
		};

		/**
		 * Select all options.  Does nothing if the listbox does not allow multiple selections.
		 * Redraws the gadget and raises a value changed event.
		 */
		virtual inline void selectAllOptions() {
			_listbox->selectAllOptions();
		};

		/**
		 * Deselect all options.
		 * Redraws the gadget and raises a value changed event.
		 */
		virtual inline void deselectAllOptions() {
			_listbox->deselectAllOptions();
		};

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
		virtual inline const s32 getSelectedIndex() const {
			return _listbox->getSelectedIndex();
		};

		/**
		 * Sets the selected index.  Specify -1 to select nothing.  Resets any
		 * other selected items to deselected.
		 * Redraws the gadget and raises a value changed event.
		 * @param index The selected index.
		 */
		virtual inline void setSelectedIndex(const s32 index) {
			_listbox->setSelectedIndex(index);
		};

		/**
		 * Get the selected option.  Returns NULL if nothing is selected.
		 * @return The selected option.
		 */
		virtual inline const ListData::ListDataItem* getSelectedOption() const {
			return _listbox->getSelectedOption();
		};
		
		/**
		 * Sets whether multiple selections are possible or not.
		 * Does not redraw the gadget.
		 * @param allowMultipleSelections True to allow multiple selections.
		 */
		virtual inline void setAllowMultipleSelections(const bool allowMultipleSelections) {
			_listbox->setAllowMultipleSelections(allowMultipleSelections);
		};

		/**
		 * Resize the scrolling canvas to encompass all options.
		 * Does not redraw the gadget.
		 */
		virtual inline void resizeCanvas() {
			_listbox->resizeCanvas();
		}

		/**
		 * Get the selected index.  Returns -1 if nothing is selected.
		 * @return The selected index.
		 */
		virtual inline const ListData::ListDataItem* getOption(const s32 index) const {
			return _listbox->getOption(index);
		};

		/**
		 * Sort the options alphabetically by the text of the options.
		 */
		virtual inline void sort() {
			_listbox->sort();
		};

		/**
		 * Get the total number of options.
		 * @return The number of options.
		 */
		virtual inline const s32 getOptionCount() const {
			return _listbox->getOptionCount();
		};

		/**
		 * Get the height of a single option.
		 * @return The height of an option.
		 */
		virtual inline const u16 getOptionHeight() const {
			return _listbox->getOptionHeight();
		};

		/**
		 * Handles events raised by its sub-gadgets.
		 */
		virtual bool handleEvent(const EventArgs& e);

		/**
		 * Resize the textbox to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		virtual bool resize(u16 width, u16 height);

		/**
		 * Set the font used in the textbox.
		 * @param font Pointer to the new font.
		 */
		virtual void setFont(FontBase* font);

		/**
		 * Sets the outline type of the list box.
		 * @param outline The outline type.
		 */
		inline void setOutlineType(const OutlineType outline) {
			_listbox->setOutlineType(outline);
		};

		/**
		 * Sets whether or not items added to the list are automatically sorted on insert or not.
		 * @param sortInsertedItems True to enable sort on insertion.
		 */
		virtual inline void setSortInsertedItems(const bool sortInsertedItems) { _listbox->setSortInsertedItems(sortInsertedItems); };

	protected:
		ListBox* _listbox;									/**< Pointer to the list box */
		ScrollbarVertical* _scrollbar;						/**< Pointer to the scrollbar */
		u8 _scrollbarWidth;									/**< Width of the scrollbar */

		/**
		 * Destructor.
		 */
		virtual inline ~ScrollingListBox() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ScrollingListBox(const ScrollingListBox& scrollingListBox) : Gadget(scrollingListBox) { };
	};
}

#endif
