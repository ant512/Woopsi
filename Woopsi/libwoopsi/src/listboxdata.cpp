#include <string.h>
#include "listboxdata.h"

using namespace WoopsiUI;

ListBoxData::ListBoxData() : ListData() { }

void ListBoxData::addItem(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	
	// Create new option
	ListBoxDataItem* item = new ListBoxDataItem(text, value, false, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour);
	
	// Determine insert type
	if (_sortInsertedItems) {
		
		// Sorted insert
		_items.insert(getSortedInsertionIndex(item), item);
	} else {

		// Append
		_items.push_back(item);
	}

	raiseDataChangedEvent();
}
