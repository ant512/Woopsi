#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <nds.h>

template <class T>
class LinkedListIterator;

/**
 * Template linked list item.
 */
template <class T>
struct LinkedListItem {
	T data;
	LinkedListItem* next;
	LinkedListItem* previous;
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
class LinkedList {
public:

	/**
	 * Constructor.
	 */
	LinkedList();

	/**
	 * Destructor.
	 */
	~LinkedList();

	/**
	 * Get the size of the list.
	 * @return The size of the list.
	 */
	u32 size() const;

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
	void insert(const u32 index, const T &value);

	/**
	 * Remove the last element from the list.
	 */
	void pop_back();

	/**
	 * Erase a single value at the specified index
	 */
	void erase(const u32 index);

	/**
	 * Get a value at the specified location.  Does not perform bounds checking.
	 * @param index The index of the desired value.
	 * @return The value at the specified index.
	 */
	T& at(const u32 index);

	/**
	 * Check if the list has any data.
	 * @return True if the list is empty.
	 */
	bool empty();

	/**
	 * Remove all data.
	 */
	void clear();

	/**
	 * Overload the [] operator to allow array-style access.
	 * @param index The index to retrieve.
	 * @return The value.
	 */
	T& operator[](const u32 index);

	/**
	 * Get an iterator for this list.
	 * @return An iterator for the list.
	 */
	LinkedListIterator<T>* getIterator();

private:
	friend class LinkedListIterator<T>;

	/**
	 * Get a pointer to the head item.
	 */
	LinkedListItem<T>* getHead();

	/**
	 * Get a pointer to the foot item.
	 */
	LinkedListItem<T>* getFoot();

	LinkedListItem<T>* _head;
	LinkedListItem<T>* _foot;
	u32 _size;
};

template <class T>
LinkedList<T>::LinkedList() {
	_size = 0;
	_head = NULL;
	_foot = NULL;
}

template <class T>
LinkedList<T>::~LinkedList() {
	clear();
}

template <class T>
u32 LinkedList<T>::size() const {
	return _size;
}

template <class T>
void LinkedList<T>::push_back(const T &value) {

	if (_head == NULL) {
		
		// Create first list item
		_head = new LinkedListItem<T>;
		_head->data = value;
		_head->previous = NULL;
		_head->next = NULL;

		_foot = _head;
	} else {

		// Append new list item
		LinkedListItem<T>* item = new LinkedListItem<T>;
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
void LinkedList<T>::pop_back() {
	if (_size >= 1) {
		// Store a pointer to the new foot
		LinkedListItem<T>* newFoot = _foot->previous;

		// Delete the existing foot
		delete _foot;

		// Update the foot pou32ers
		_foot = newFoot;
		_foot->next = NULL;

		_size--;
	}
}

template <class T>
void LinkedList<T>::insert(const u32 index, const T &value) {

	// Quick insert if list is empty or if index exceeds bounds
	if ((_head == NULL) || (index >= _size)) {
		push_back(value);
	}

	// Locate the requested index - this could be optimised
	// by looping from back to front if the desired index is over
	// half-way through the list.
	u32 i = 0;
	LinkedListItem<T>* item = _head;

	while ((i < index) && (item != NULL)) {
		item = item->next;
		i++;
	}

	// Insert a new item
	LinkedListItem<T>* newItem = new LinkedListItem<T>;
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
void LinkedList<T>::erase(const u32 index) {

	// Abort if index exceeds bounds
	if (index >= _size) return;

	// Shortcut if erasing last item
	if (index == _size - 1) {
		pop_back();
		return;
	}

	// Shortcut if erasing first item
	if (index == 0) {
		LinkedListItem<T>* newHead = _head->next;

		delete _head;

		if (newHead != NULL) {
			newHead->previous = NULL;
			_head = newHead;
		}

		return;
	}

	// No shortcuts possible; locate the requested index
	u32 i = 0;
	LinkedListItem<T>* item = _head;

	while ((i < index) && (item != NULL)) {
		item = item->next;
		i++;
	}

	// Rewire pou32ers
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
T& LinkedList<T>::at(const u32 index) {
	u32 i = 0;
	LinkedListItem<T>* item = _head;

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
bool LinkedList<T>::empty() {
	return (_size == 0);
}

template <class T>
T& LinkedList<T>::operator[](const u32 index) {
	return at(index);
}

template <class T>
void LinkedList<T>::clear() {
	LinkedListItem<T>* item = _head;
	LinkedListItem<T>* nextItem = NULL;
	
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
LinkedListIterator<T>* LinkedList<T>::getIterator() {
	return new LinkedListIterator<T>(this);
}

template <class T>
LinkedListItem<T>* LinkedList<T>::getHead() {
	return _head;
}


template <class T>
LinkedListItem<T>* LinkedList<T>::getFoot() {
	return _foot;
}

#endif
