#ifndef _LIST_DATA_ITEM_H_
#define _LIST_DATA_ITEM_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Class representing a data item within a list.  Intended for use within the ListData
	 * class.
	 */
	class ListDataItem {
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
		ListDataItem(const char* text, const u32 value,
			const u16 normalTextColour,
			const u16 normalBackColour,
			const u16 selectedTextColour,
			const u16 selectedBackColour);
		
		/**
		 * Destructor.
		 */
		virtual ~ListDataItem();

		/**
		 * Get the item's text.
		 * @return The item's text.
		 */
		inline const char* getText() const { return _text; };

		/**
		 * Get the item's value.
		 * @return The item's value.
		 */
		inline const u32 getValue() const { return _value; };

		/**
		 * Get the item's selection state.
		 * @return True if the item is selected; false if not.
		 */
		inline const bool isSelected() const { return _isSelected; };

		/**
		 * Set the item's selection state.
		 * @param selected True to select the item; false to deselect it.
		 */
		inline void setSelected(bool selected) { _isSelected = selected; };

		/**
		 * Compare the item with another.  Comparison is based on the text of
		 * the item.  Returns 0 if the text in the two items is the same,
		 * a value less than 0 if this item is less than the argument, and
		 * a value greater than 0 if this item is greater than the argument.
		 * @param item An item to compare this object with.
		 * @return 0 if the text in the two items is the same,
		 * a value less than 0 if this item is less than the argument, and
		 * a value greater than 0 if this item is greater than the argument.
		 */
		virtual s8 compareTo(const ListDataItem* item) const;

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
		char* _text;							/**< Text to display for option. */
		u32 _value;								/**< Option value. */
		bool _isSelected;						/**< True if the option is selected. */
		u16 _normalTextColour;					/**< Colour used for text when not selected. */
		u16 _normalBackColour;					/**< Colour used for background when not selected. */
		u16 _selectedTextColour;				/**< Colour used for text when selected. */
		u16 _selectedBackColour;				/**< Colour used for background when selected. */
	};
}

#endif
