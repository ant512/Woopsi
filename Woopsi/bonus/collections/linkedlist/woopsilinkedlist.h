#ifndef _WOOPSI_LINKED_LIST_H_
#define _WOOPSI_LINKED_LIST_H_

#include <nds.h>

template <class T>
class WoopsiLinkedListIterator;

/**
 * Template linked list item.
 */
template <class T>
struct WoopsiLinkedListItem {
	T data;
	WoopsiLinkedListItem* next;
	WoopsiLinkedListItem* previous;
};

/**
 * Linked list class.  Provides simulated random access to data.  Sequential
 * access is provided by the LinkedListIterator class, an instance of which
 * can be created with the getIterator() function.
 *
 * Use the DynamicList instead if you intend to perform a lot of random access
 * on the data in the list.  Random access is provided by iterating through
 * the data until the correct item is found, which will decrease performance
 * when dealing with random access of large lists.
 */
template <class T>
class WoopsiLinkedList {
public:

	/**
	 * Constructor.
	 */
	inline WoopsiLinkedList();

	/**
	 * Destructor.
	 */
	inline ~WoopsiLinkedList();

	/**
	 * Get the size of the list.
	 * @return The size of the list.
	 */
	inline s32 size() const;

	/**
	 * Add a value to the end of the list.
	 * @param value The value to add to the list.
	 */
	void push_back(const T &value);

	/**
	 * Insert a value into the list.
	 * @param index The index to insert into.
	 * @param value The value to insert.
	 */
	void insert(const s32 index, const T &value);

	/**
	 * Remove the last element from the list.
	 */
	void pop_back();

	/**
	 * Erase a single value at the specified index
	 */
	void erase(const s32 index);

	/**
	 * Get a value at the specified location.  Does not perform bounds checking.
	 * @param index The index of the desired value.
	 * @return The value at the specified index.
	 */
	inline T& at(const s32 index) const;

	/**
	 * Check if the list has any data.
	 * @return True if the list is empty.
	 */
	inline bool empty() const;

	/**
	 * Remove all data.
	 */
	void clear();

	/**
	 * Overload the [] operator to allow array-style access.
	 * @param index The index to retrieve.
	 * @return The value.
	 */
	T& operator[](const s32 index) const;

	/**
	 * Get an iterator for this list.
	 * @return An iterator for the list.
	 */
	WoopsiLinkedListIterator<T>* getIterator();

private:
	friend class WoopsiLinkedListIterator<T>;

	/**
	 * Get a pointer to the head item.
	 */
	WoopsiLinkedListItem<T>* getHead();

	/**
	 * Get a pointer to the foot item.
	 */
	WoopsiLinkedListItem<T>* getFoot();

	WoopsiLinkedListItem<T>* _head;
	WoopsiLinkedListItem<T>* _foot;
	u32 _size;
};

template <class T>
WoopsiLinkedList<T>::WoopsiLinkedList() {
	_size = 0;
	_head = NULL;
	_foot = NULL;
}

template <class T>
WoopsiLinkedList<T>::~WoopsiLinkedList() {
	clear();
}

template <class T>
s32 WoopsiLinkedList<T>::size() const {
	return _size;
}

template <class T>
void WoopsiLinkedList<T>::push_back(const T &value) {

	if (_head == NULL) {
		
		// Create first list item
		_head = new LinkedListItem<T>;
		_head->data = value;
		_head->previous = NULL;
		_head->next = NULL;

		_foot = _head;
	} else {

		// Append new list item
		WoopsiLinkedListItem<T>* item = new WoopsiLinkedListItem<T>;
		item->data = value;
		item->previous = _foot;
		item->next = NULL;

		// Update foot
		_foot->next = item;

		// Swap pointer
		_foot = item;
	}

	// Remember we've filled a slot
	_size++;
}

template <class T>
void WoopsiLinkedList<T>::pop_back() {
	if (_size >= 1) {
		// Store a pointer to the new foot
		WoopsiLinkedListItem<T>* newFoot = _foot->previous;

		// Delete the existing foot
		delete _foot;

		// Update the foot pointers
		_foot = newFoot;

		if (_foot != NULL) {
			_foot->next = NULL;
		}

		_size--;
	}
}

template <class T>
void WoopsiLinkedList<T>::insert(const s32 index, const T &value) {

	// Quick insert if list is empty or if index exceeds bounds
	if ((_head == NULL) || (index >= _size)) {
		push_back(value);
	}

	// Locate the requested index - this could be optimised
	// by looping from back to front if the desired index is over
	// half-way through the list.
	s32 i = 0;
	WoopsiLinkedListItem<T>* item = _head;

	while ((i < index) && (item != NULL)) {
		item = item->next;
		i++;
	}

	// Insert a new item
	LinkedListItem<T>* newItem = new WoopsiLinkedListItem<T>;
	newItem->data = value;
	newItem->previous = item->previous;
	newItem->next = item;

	if (newItem->previous != NULL) {
		newItem->previous->next = newItem;
	}

	// Update existing pou32ers
	item->previous = newItem;

	// Remember we've filled a slot
	_size++;
}

template <class T>
void WoopsiLinkedList<T>::erase(const s32 index) {

	// Abort if index exceeds bounds
	if (index >= _size) return;

	// Shortcut if erasing last item
	if (index == _size - 1) {
		pop_back();
		return;
	}

	// Shortcut if erasing first item
	if (index == 0) {
		WoopsiLinkedListItem<T>* newHead = _head->next;

		delete _head;

		if (newHead != NULL) {
			newHead->previous = NULL;
			_head = newHead;
			_foot = _head;
		}

		return;
	}

	// No shortcuts possible; locate the requested index
	s32 i = 0;
	WoopsiLinkedListItem<T>* item = _head;

	while ((i < index) && (item != NULL)) {
		item = item->next;
		i++;
	}

	// Rewire pointers
	if (item->previous != NULL) {
		item->previous->next = item->next;
	}

	if (item->next != NULL) {
		item->next->previous = item->previous;
	}

	// Delete the item
	delete item;

	// Remember we've removed a slot
	_size--;
}

template <class T>
T& WoopsiLinkedList<T>::at(const s32 index) const {
	s32 i = 0;
	WoopsiLinkedListItem<T>* item = _head;

	// Locate the requested index - this could be optimised
	// by looping from back to front if the desired index is over
	// half-way through the list.
	while ((i < index) && (item != NULL)) {
		item = item->next;
		i++;
	}

	return item->data;
}

template <class T>
bool WoopsiLinkedList<T>::empty() const {
	return (_size == 0);
}

template <class T>
T& WoopsiLinkedList<T>::operator[](const s32 index) const {
	return at(index);
}

template <class T>
void WoopsiLinkedList<T>::clear() {
	WoopsiLinkedListItem<T>* item = _head;
	WoopsiLinkedListItem<T>* nextItem = NULL;
	
	// Scan through list deleting each item
	while (item != NULL) {
		nextItem = item->next;
		
		delete item;

		item = nextItem;
	}

	_head = NULL;
	_foot = NULL;

	_size = 0;
}

template <class T>
LinkedListIterator<T>* WoopsiLinkedList<T>::getIterator() {
	return new LinkedListIterator<T>(this);
}

template <class T>
LinkedListItem<T>* WoopsiLinkedList<T>::getHead() {
	return _head;
}

template <class T>
LinkedListItem<T>* WoopsiLinkedList<T>::getFoot() {
	return _foot;
}

#endif
