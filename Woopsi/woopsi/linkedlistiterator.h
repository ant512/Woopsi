#ifndef _LINKED_LIST_ITERATOR_H_
#define _LINKED_LIST_ITERATOR_H_

#include <nds.h>

#include "linkedlist.h"

/**
 * Class providing sequential access to a linked list.  Rather than force
 * the coder to work with the raw list data, this class abstracts it away
 * with ADO RecordSet-style functionality.
 *
 * This class cannot be instantiated; rather, an instance of the class
 * is created by calling getIterator() on an instance of the LinkedList.
 */
template <class T>
class LinkedListIterator {
public:

	/**
	 * Destructor.
	 */
	~LinkedListIterator();

	/**
	 * Move the iterator pointer to the first item in the list.
	 */
	void moveToFirst();
	
	/**
	 * Move the iterator pointer to the last item in the list.
	 */
	void moveToLast();

	/**
	 * Move the iterator pointer to the next item in the list.
	 * @return True if the iterator could move to the next item.
	 */
	bool moveToNext();

	/**
	 * Move the iterator pointer to the previous item in the list.
	 * @return True if the iterator could move to the previous item.
	 */
	bool moveToPrevious();

	/**
	 * Get the value of the iterator.
	 * @return The value of the iterator.
	 */
	T& iterator();

private:
	friend class LinkedList<T>;

	LinkedList<T>* _list;
	LinkedListItem<T>* _iterator;

	/**
	 * Constructor is private to prevent creation outside of the linked list.
	 */
	LinkedListIterator(LinkedList<T>* list);
};

template <class T>
LinkedListIterator<T>::LinkedListIterator(LinkedList<T>* list) {
	_list = list;

	// Point the iterator to the top of the list
	_iterator = list->getHead();
}

template <class T>
LinkedListIterator<T>::~LinkedListIterator() {
}

template <class T>
void LinkedListIterator<T>::moveToFirst() {
	_iterator = _list->getHead();
}

template <class T>
void LinkedListIterator<T>::moveToLast() {
	_iterator = _list->getFoot();
}

template <class T>
T& LinkedListIterator<T>::iterator() {
	return _iterator->data;
}

template <class T>
bool LinkedListIterator<T>::moveToNext() {
	if ((_iterator != NULL) && (_iterator->next != NULL)) {
		_iterator = _iterator->next;
		return true;
	}

	return false;
}

template <class T>
bool LinkedListIterator<T>::moveToPrevious() {
	if ((_iterator != NULL) && (_iterator->previous != NULL)) {
		_iterator = _iterator->previous;
		return true;
	}

	return false;
}

#endif
