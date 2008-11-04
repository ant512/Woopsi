#ifndef _LIST_DATA_H_
#define _LIST_DATA_H_

#include "dynamicarray.h"

namespace WoopsiUI {

	/**
	 * Class representing a data item within a list.
	 */
	class ListData {
	public:

		/**
		 * Struct defining a single item.
		 */
		typedef struct {
			char* text;							/**< Text to display for option. */
			u32 value;							/**< Option value. */
			u16 normalTextColour;				/**< Colour used for text when not selected. */
			u16 normalBackColour;				/**< Colour used for background when not selected. */
			u16 selectedTextColour;				/**< Colour used for text when selected. */
			u16 selectedBackColour;				/**< Colour used for background when selected. */
			bool selected;						/**< True if the option is selected. */
		} ListDataItem;

		/**
		 * Constructor.
		 */
		ListData();

		/**
		 * Destructor.
		 */
		virtual ~ListData();

		/**
		 * Add a new item.
		 * @param text Text to show in the option.
		 * @param value The value of the option.
		 * @param normalTextColour Colour to draw the text with when not selected.
		 * @param normalBackColour Colour to draw the background with when not selected.
		 * @param selectedTextColour Colour to draw the text with when selected.
		 * @param selectedBackColour Colour to draw the background with when selected.
		 */
		virtual void addItem(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour);

		/**
		 * Remove an item by its index..
		 * @param index The index of the option to remove.
		 */
		virtual void removeItem(const s32 index);

		/**
		 * Select an item by its index.  Does not deselect any other selected items.
		 * @param index The index of the item to select.
		 */
		virtual void selectItem(const s32 index);

		/**
		 * Select an item by its index.  Does not deselect any other selected items.
		 * @param index The index of the item to select.
		 */
		virtual void deselectItem(const s32 index);

		/**
		 * Remove all items.
		 */
		virtual void removeAllItems();

		/**
		 * Get the selected index.  Returns -1 if nothing is selected.  If more than one
		 * item is selected, the index of the first selected item is returned.
		 * @return The selected index.
		 */
		virtual const s32 getSelectedIndex() const;

		/**
		 * Sets the selected index.  Specify -1 to select nothing.  Resets any
		 * other selected items to deselected.
		 * @param index The selected index.
		 */
		virtual void setSelectedIndex(const s32 index);

		/**
		 * Get the selected item.  Returns NULL if nothing is selected.
		 * @return The selected option.
		 */
		virtual const ListDataItem* getSelectedItem() const;

		/**
		 * Sets whether multiple selections are possible or not.
		 * @param allowMultipleSelections True to allow multiple selections.
		 */
		virtual inline void setAllowMultipleSelections(const bool allowMultipleSelections) { _allowMultipleSelections = allowMultipleSelections; };

		/**
		 * Get the specified item.
		 * @return The specified item.
		 */
		virtual inline ListDataItem* getItem(const s32 index) { return _items[index]; };

		/**
		 * Sort the items alphabetically by the text of the items.
		 */
		virtual void sort();

		/**
		 * Get the total number of items.
		 * @return The number of items.
		 */
		virtual inline const s32 getItemCount() const {
			return _items.size();
		}

		/**
		 * Select all items.  Does nothing if the list does not allow multiple selections.
		 */
		virtual void selectAllItems();

		/**
		 * Deselect all items.
		 */
		virtual void deselectAllItems();

		/**
		 * Select or deselect an item by its index.  Does not deselect any other selected items.
		 * Set index to -1 to select nothing.
		 * @param index The index of the item to select.
		 * @param selected True to select the item, false to deselect it.
		 */
		virtual void setItemSelected(const s32 index, const bool selected);

		/**
		 * Returns whether multiple selections are possible or not.
		 * @return True if multiple selections are allowed.
		 */
		virtual inline const bool allowsMultipleSelections() const { return _allowMultipleSelections; };

		/**
		 * Sets whether or not items added to the list are automatically sorted on insert or not.
		 * @param sortInsertedItems True to enable sort on insertion.
		 */
		virtual inline void setSortInsertedItems(const bool sortInsertedItems) { _sortInsertedItems = sortInsertedItems; };

	protected:
		DynamicArray<ListDataItem*> _items;			/**< Collection of list data items. */
		bool _allowMultipleSelections;				/**< If true, multiple options can be selected. */
		bool _sortInsertedItems;					/**< Automatically sorts items on insertion if true. */

		/**
		 * Quick sort the items alphabetically by the text of the items.
		 * @param start The index to start sorting at.
		 * @param end The index to stop sorting at.
		 */
		virtual void quickSort(const s32 start, const s32 end);

		/**
		 * Swap the locations of two items in the array.
 		 * @param index1 The index of the first item to swap.
		 * @param index2 The index of the second item to swap.
		 */
		virtual void swapItems(const s32 index1, const s32 index2);

		/**
		 * Return the index that an item with the supplied text should be
		 * inserted at to maintain a sorted list of data.
		 * @param text The text of the item.
		 * @return The index that the item should be imserted into at.
		 */
		const s32 getInsertionIndex(const char* text) const;
	};
}

#endif
