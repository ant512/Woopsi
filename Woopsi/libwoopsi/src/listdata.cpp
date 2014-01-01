#include <string.h>
#include "listdata.h"

using namespace WoopsiUI;

ListData::ListData() {
	_allowMultipleSelections = true;
	_sortInsertedItems = false;
}

ListData::~ListData() {
	
	// Delete all option data
	for (s32 i = 0; i < _items.size(); i++) {
		delete _items[i];
	}
	
	_items.clear();
}

void ListData::addItem(ListDataItem* item) {

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

void ListData::addItem(const WoopsiString& text, const u32 value) {
	
	// Create new option
	addItem(new ListDataItem(text, value));
}

void ListData::removeItem(const s32 index) {

	// Bounds check
	if (index < _items.size()) {

		// Delete the option
		delete _items[index];

		// Erase the option from the list
		_items.erase(index);

		raiseDataChangedEvent();
	}
}

void ListData::setSelectedIndex(const s32 index) {
	setItemSelected(index, true);
}

const s32 ListData::getSelectedIndex() const {

	// Get the first selected index
	for (s32 i = 0; i < _items.size(); i++) {
		if (_items[i]->isSelected()) return i;
	}
	
	return -1;
}

const ListDataItem* ListData::getSelectedItem() const {
	
	// Get the first selected option
	s32 index = getSelectedIndex();
	
	if (index > -1) {
		return _items[index];
	}
	
	return NULL;
}

void ListData::selectItem(const s32 index) {
	setItemSelected(index, true);
}

void ListData::deselectItem(const s32 index) {
	setItemSelected(index, false);
}

void ListData::setItemSelected(const s32 index, bool selected) {

	// Deselect old options if we're making an option selected and we're not a multiple list
	if (((!_allowMultipleSelections) || (index == -1)) && (selected)) {
		for (s32 i = 0; i < _items.size(); i++) {
			_items[i]->setSelected(false);
		}
	}

	// Select or deselect the new option
	if ((index > -1) && (index < _items.size())) {
		_items[index]->setSelected(selected);
	}

	raiseSelectionChangedEvent();
}

void ListData::deselectAllItems() {
	for (s32 i = 0; i < _items.size(); i++) {
		_items[i]->setSelected(false);
	}

	raiseSelectionChangedEvent();
}

void ListData::selectAllItems() {
	if (_allowMultipleSelections) {
		for (s32 i = 0; i < _items.size(); i++) {
			_items[i]->setSelected(true);
		}

		raiseSelectionChangedEvent();
	}
}

void ListData::sort() {
	quickSort(0, _items.size() - 1);
	
	raiseDataChangedEvent();
}

void ListData::quickSort(const s32 start, const s32 end) {
	if (end > start) {

		int left = start;
		int right = end;

		ListDataItem* pivot = _items[(start + end) >> 1];

		do {
			while ((pivot->compareTo(_items[left]) > 0) && (left < end)) left++;
			while ((pivot->compareTo(_items[right]) < 0) && (right > start)) right--;

			if (left > right) break;

			swapItems(left, right);
			left++;
			right--;
		} while (left <= right);

		quickSort(start, right);
		quickSort(left, end);
	}
}

void ListData::swapItems(const s32 index1, const s32 index2) {
	ListDataItem* tmp = _items[index1];
	_items[index1] = _items[index2];
	_items[index2] = tmp;
}

void ListData::removeAllItems() {

	// Delete all option data
	for (s32 i = 0; i < _items.size(); i++) {
		delete _items[i];
	}
	
	_items.clear();

	raiseDataChangedEvent();
}

const s32 ListData::getSortedInsertionIndex(const ListDataItem* item) const {

	s32 i = 0;

	// Locate slot where new option should go
	while ((i < _items.size()) && (item->compareTo(_items[i]) > 0)) {
		i++;
	}

	return i;
}

void ListData::raiseDataChangedEvent() {
	_listDataEventHandler->handleListDataChangedEvent(*this);
}

void ListData::raiseSelectionChangedEvent() {
	_listDataEventHandler->handleListDataSelectionChangedEvent(*this);
}
