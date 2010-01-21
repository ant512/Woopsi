#ifndef _FILE_LISTBOX_DATA_ITEM_H_
#define _FILE_LISTBOX_DATA_ITEM_H_

#include <nds.h>
#include "listboxdataitem.h"

namespace WoopsiUI {

	/**
	 * Class representing a data item within a file requester list.
	 */
	class FileListBoxDataItem : public ListBoxDataItem {
	public:

		/**
		 * Constructor.
		 * @param text The text to display in the item.
		 * @param value The value of the item.
  		 * @param normalTextColour Colour to draw the text with when not selected.
		 * @param normalBackColour Colour to draw the background with when not selected.
		 * @param selectedTextColour Colour to draw the text with when selected.
		 * @param selectedBackColour Colour to draw the background with when selected.
		 * @param isDirectory True indicates that the item is a directory; false
		 * indicates that it is a file.
		 */
		FileListBoxDataItem(const char* text, const u32 value,
			const u16 normalTextColour,
			const u16 normalBackColour,
			const u16 selectedTextColour,
			const u16 selectedBackColour,
			const bool isDirectory);

		/**
		 * Check if the item is a directory.
		 * @return True if the item is a directory; false if the item is a file.
		 */
		inline const bool isDirectory() const { return _isDirectory; };

		/**
		 * Compare the item with another.  Comparison is based on the type of
		 * the item - directories come before files.  If the two items are of
		 * the same type, the default text-based comparison is used.
		 * Returns 0 if the text in the two items is the same,
		 * a value less than 0 if this item is less than the argument, and
		 * a value greater than 0 if this item is greater than the argument.
		 * @param item An item to compare this object with.
		 * @return 0 if the text in the two items is the same,
		 * a value less than 0 if this item is less than the argument, and
		 * a value greater than 0 if this item is greater than the argument.
		 */
		virtual s8 compareTo(const ListDataItem* item) const;

	private:
		bool _isDirectory;						/**< True if the option is a directory. */
	};
}

#endif
