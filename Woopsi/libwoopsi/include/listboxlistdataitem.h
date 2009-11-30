#ifndef _LIST_BOX_LIST_DATA_ITEM_H_
#define _LIST_BOX_LIST_DATA_ITEM_H_

#include <nds.h>
#include "listdataitem.h"

namespace WoopsiUI {

	/**
	 * Class representing a data item within a ListBox.
	 */
	class ListBoxListDataItem : public ListDataItem {
	public:

		/**
		 * Constructor.
		 * @param text The text to display in the item.
		 * @param value The value of the item.
  		 * @param normalTextColour Colour to draw the text with when not selected.
		 * @param normalBackColour Colour to draw the background with when not selected.
		 * @param selectedTextColour Colour to draw the text with when selected.
		 * @param selectedBackColour Colour to draw the background with when selected.
		 */
		ListBoxListDataItem(const char* text, const u32 value,
			const u16 normalTextColour,
			const u16 normalBackColour,
			const u16 selectedTextColour,
			const u16 selectedBackColour);

		/**
		 * Get the colour used for text when the item is unselected.
		 * @return The text colour when the item is unselected.
		 */
		inline u16 getNormalTextColour() const { return _normalTextColour; };

		/**
		 * Get the colour used for the background when the item is unselected.
		 * @return The background colour when the item is unselected.
		 */
		inline u16 getNormalBackColour() const { return _normalBackColour; };

		/**
		 * Get the colour used for text when the item is selected.
		 * @return The text colour when the item is selected.
		 */
		inline u16 getSelectedTextColour() const { return _selectedTextColour; };

		/**
		 * Get the colour used for the background when the item is selected.
		 * @return The background colour when the item is selected.
		 */
		inline u16 getSelectedBackColour() const { return _selectedBackColour; };

	private:
		u16 _normalTextColour;					/**< Colour used for text when not selected. */
		u16 _normalBackColour;					/**< Colour used for background when not selected. */
		u16 _selectedTextColour;				/**< Colour used for text when selected. */
		u16 _selectedBackColour;				/**< Colour used for background when selected. */
	};
}

#endif
