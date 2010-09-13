#ifndef _LISTBOX_BASE_H_
#define _LISTBOX_BASE_H_

#include "listboxdataitem.h"

namespace WoopsiUI {

	/**
	 * Defines the interface for ListBox classes.
	 */
	class ListBoxBase {
	public:

		/**
		 * Add a new option to the gadget using default colours.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 */
		virtual void addOption(const WoopsiString& text, const u32 value) = 0;

		/**
		 * Add an option to the gadget.
		 * @param option The option to add.
		 */
		virtual void addOption(ListBoxDataItem* option) = 0;

		/**
		 * Remove an option from the gadget by its index.
		 * @param index The index of the option to remove.
		 */
		virtual void removeOption(const s32 index) = 0;

		/**
		 * Remove all options from the gadget.
		 */
		virtual void removeAllOptions() = 0;

		/**
		 * Add a new option to the gadget.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 * @param normalTextColour Colour to draw the text with when not
		 * selected.
		 * @param normalBackColour Colour to draw the background with when not
		 * selected.
		 * @param selectedTextColour Colour to draw the text with when selected.
		 * @param selectedBackColour Colour to draw the background with when
		 * selected.
		 */
		virtual void addOption(const WoopsiString& text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) = 0;

		/**
		 * Select an option by its index.
		 * Raises a value changed event.
		 * @param index The index of the option to select.
		 */
		virtual void selectOption(const s32 index) = 0;

		/**
		 * Select an option by its index.
		 * Raises a value changed event.
		 * @param index The index of the option to select.
		 */
		virtual void deselectOption(const s32 index) = 0;

		/**
		 * Select all options.  Does nothing if the listbox does not allow
		 * multiple selections.
		 * Raises a value changed event.
		 */
		virtual void selectAllOptions() = 0;

		/**
		 * Deselect all options.
		 * Raises a value changed event.
		 */
		virtual void deselectAllOptions() = 0;

		/**
		 * Get the selected index.  Returns -1 if nothing is selected.  If more
		 * than one option is selected, the index of the first selected option
		 * is returned.
		 * @return The selected index.
		 */
		virtual const s32 getSelectedIndex() const = 0;

		/**
		 * Sets the selected index.  Specify -1 to select nothing.  Resets any
		 * other selected options to deselected.
		 * Raises a value changed event.
		 * @param index The selected index.
		 */
		virtual void setSelectedIndex(const s32 index) = 0;

		/**
		 * Get the selected option.  Returns NULL if nothing is selected.
		 * @return The selected option.
		 */
		virtual const ListBoxDataItem* getSelectedOption() const = 0;
		
		/**
		 * Sets whether multiple selections are possible or not.
		 * @param allowMultipleSelections True to allow multiple selections.
		 */
		virtual void setAllowMultipleSelections(const bool allowMultipleSelections) = 0;

		/**
		 * Sets whether multiple selections are possible or not.
		 * @return True if multiple selections are allowed.
		 */
		virtual const bool allowsMultipleSelections() const = 0;

		/**
		 * Get the specified option.
		 * @return The specified option.
		 */
		virtual const ListBoxDataItem* getOption(const s32 index) = 0;

		/**
		 * Sort the options alphabetically by the text of the options.
		 */
		virtual void sort() = 0;

		/**
		 * Get the total number of options.
		 * @return The number of options.
		 */
		virtual const s32 getOptionCount() const = 0;

		/**
		 * Get the height of a single option.
		 * @return The height of an option.
		 */
		virtual const u16 getOptionHeight() const = 0;

		/**
		 * Sets whether or not items added to the list are automatically sorted
		 * on insert or not.
		 * @param sortInsertedItems True to enable sort on insertion.
		 */
		virtual void setSortInsertedItems(const bool sortInsertedItems) = 0;

		/**
		 * Destructor.
		 */
		virtual ~ListBoxBase() { };
	};
}

#endif
