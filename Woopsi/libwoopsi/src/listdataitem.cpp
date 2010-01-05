#include "listdataitem.h"

using namespace WoopsiUI;

ListDataItem::ListDataItem(const WoopsiString& text, const u32 value) {

	_text = text;
	_value = value;
	_isSelected = false;
}

ListDataItem::~ListDataItem() {
}

s8 ListDataItem::compareTo(const ListDataItem* item) const {
	return _text.compareTo(item->getText());
}
