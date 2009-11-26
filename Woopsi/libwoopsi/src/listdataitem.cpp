#include "listdataitem.h"

using namespace WoopsiUI;

ListDataItem::ListDataItem(const char* text, const u32 value,
						   const u16 normalTextColour,
						   const u16 normalBackColour,
						   const u16 selectedTextColour,
						   const u16 selectedBackColour) {

	_text = new char[strlen(text) + 1];
	strcpy(_text, text);

	_value = value;
	_isSelected = false;

	_normalTextColour = normalTextColour;
	_normalBackColour = normalBackColour;
	_selectedTextColour = selectedTextColour;
	_selectedBackColour = selectedBackColour;
}

ListDataItem::~ListDataItem() {
	delete [] _text;
}

s8 ListDataItem::compareTo(const ListDataItem* item) const {
	return strcmp(_text, item->getText());
}
