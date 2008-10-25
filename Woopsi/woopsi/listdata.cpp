#include <string.h>
#include "listdata.h"

using namespace WoopsiUI;

ListData::ListData() {
	_allowMultipleSelections = true;
	_sortInsertedItems = false;
}

ListData::~ListData() {
	removeAllItems();
}

void ListData::addItem(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	
	// Create new memory for string
	char* newText = new char[strlen(text) + 1];
	
	// Copy text
	strcpy(newText, text);
	
	// Create new option
	ListDataItem* newItem = new ListDataItem;
	newItem->text = newText;
	newItem->value = value;
	newItem->normalTextColour = normalTextColour;
	newItem->normalBackColour = normalBackColour;
	newItem->selectedTextColour = selectedTextColour;
	newItem->selectedBackColour = selectedBackColour;
	newItem->selected = false;
	
	// Determine insert type
	if (_sortInsertedItems) {
		
		// Sorted insert
		_items.insert(getInsertionIndex(text), newItem);
	} else {

		// Append
		_items.push_back(newItem);
	}
}

void ListData::removeItem(const s32 index) {

	// Bounds check
	if (index < _items.size()) {

		// Delete the option
		delete _items[index]->text;
		delete _items[index];

		// Erase the option from the list
		_items.erase(index);
	}
}

void ListData::setSelectedIndex(const s32 index) {
	setItemSelected(index, true);
}

const s32 ListData::getSelectedIndex() const {

	// Get the first selected index
	for (s32 i = 0; i < _items.size(); i++) {
		if (_items[i]->selected) return i;
	}
	
	return -1;
}

const ListData::ListDataItem* ListData::getSelectedItem() const {
	
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
			_items[i]->selected = false;
		}
	}

	// Select or deselect the new option
	if ((index > -1) && (index < _items.size())) {
		_items[index]->selected = selected;
	}
}

void ListData::deselectAllItems() {
	for (s32 i = 0; i < _items.size(); i++) {
		_items[i]->selected = false;
	}
}

void ListData::selectAllItems() {
	if (_allowMultipleSelections) {
		for (s32 i = 0; i < _items.size(); i++) {
			_items[i]->selected = true;
		}
	}
}

void ListData::sort() {
	quickSort(0, _items.size() - 1);
}

void ListData::quickSort(const s32 start, const s32 end) {
	if (end > start) {

		int left = start;
		int right = end;

		char* pivot = _items[(start + end) >> 1]->text;

		do {
			while ((strcmp(_items[left]->text, pivot) < 0) && (left < end)) left++;
			while ((strcmp(_items[right]->text, pivot) > 0) && (right > start)) right--;

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
		delete _items[i]->text;
		delete _items[i];
	}
	
	_items.clear();
}

const s32 ListData::getInsertionIndex(const char* text) const {

	s32 i = 0;

	// Locate slot where new option should go
	while ((i < _items.size()) && (strcmp(_items[i]->text, text) < 0)) {
		i++;
	}

	return i;
}
