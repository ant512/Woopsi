#ifndef _LIST_BASE_H_
#define _LIST_BASE_H_

#include "woopsiarray.h"
#include "listdataeventhandler.h"
#include "listdataitem.h"

namespace WoopsiUI {

	/**
	 * Class representing a list of items.  Designed to be used by the ListBox class, etc, to
	 * store its data.  Fires events to notify listeners when the list changes or a new selection
	 * is made.
	 */
	class ListData {
	public:

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
		 */
		virtual void addItem(const char* text, const u32 value);

		/**
		 * Add an existing item.  ListData becomes the owner of the option and will delete it
		 * when the list is deleted.
		 * @param item The item to add.
		 */
		virtual void addItem(ListDataItem* item);


		/**
		 * Remove an item by its index.
		 * @param index The index of the option to remove.
		 */
		virtual void removeItem(const s32 index);

		/**
		 * Select an item by its index.
		 * @param index The index of the item to select.
		 */
		virtual void selectItem(const s32 index);

		/**
		 * Deselect an item by its index.
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
		virtual inline const ListDataItem* getItem(const s32 index) const { return _items[index]; };

		/**
		 * Sort the items using their compareTo() methods.
		 */
		virtual void sort();

		/**
		 * Get the total number of items.
		 * @return The number of items.
		 */
		virtual inline const s32 getItemCount() const { return _items.size(); };

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

		/**
		 * Add an event handler.
		 * @param eventHandler The event handler to add.
		 */
		inline void addListDataEventHandler(ListDataEventHandler* eventHandler) {
			_listDataEventhandlers.push_back(eventHandler);
		}

		/**
		 * Remove an event handler.
		 * @param eventHandler The event handler to remove.
		 */
		void removeListDataEventHandler(ListDataEventHandler* eventHandler);

	protected:
		WoopsiArray<ListDataItem*> _items;							/**< Collection of list data items. */
		WoopsiArray<ListDataEventHandler*> _listDataEventhandlers;	/**< Collection of event handlers. */
		bool _allowMultipleSelections;								/**< If true, multiple options can be selected. */
		bool _sortInsertedItems;									/**< Automatically sorts items on insertion if true. */

		/**
		 * Quick sort the items using their compareTo() methods.
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
		 * Return the index that an item should be inserted at to maintain a sorted list of data.
		 * @param item The item to insert.
		 * @return The index that the item should be imserted into at.
		 */
		const s32 getSortedInsertionIndex(const ListDataItem* item) const;

		/**
		 * Raise a data changed event.
		 */
		void raiseDataChangedEvent();

		/**
		 * Raise a selection changed event.
		 */
		void raiseSelectionChangedEvent();
	};
}

#endif
