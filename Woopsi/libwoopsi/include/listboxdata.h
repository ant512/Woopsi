#ifndef _LIST_BOX_DATA_H_
#define _LIST_BOX_DATA_H_

#include "woopsiarray.h"
#include "listdataeventhandler.h"
#include "listboxdataitem.h"
#include "listdata.h"

namespace WoopsiUI {

	/**
	 * Class representing a list of items for use with the ListBox gadget.
	 */
	class ListBoxData : public ListData {
	public:

		/**
		 * Constructor.
		 */
		ListBoxData();

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
	};
}

#endif
