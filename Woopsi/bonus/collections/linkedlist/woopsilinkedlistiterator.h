#ifndef _LINKED_LIST_ITERATOR_H_
#define _LINKED_LIST_ITERATOR_H_

#include <nds.h>
#include "woopsilinkedlist.h"

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
	friend class WoopsiLinkedList<T>;

	WoopsiLinkedList<T>* _list;
	WoopsiLinkedListItem<T>* _iterator;

	/**
	 * Constructor is private to prevent creation outside of the linked list.
	 */
	WoopsiLinkedListIterator(LinkedList<T>* list);
};

template <class T>
WoopsiLinkedListIterator<T>::WoopsiLinkedListIterator(LinkedList<T>* list) {
	_list = list;

	// Point the iterator to the top of the list
	_iterator = list->getHead();
}

template <class T>
WoopsiLinkedListIterator<T>::~WoopsiLinkedListIterator() {
}

template <class T>
void WoopsiLinkedListIterator<T>::moveToFirst() {
	_iterator = _list->getHead();
}

template <class T>
void WoopsiLinkedListIterator<T>::moveToLast() {
	_iterator = _list->getFoot();
}

template <class T>
T& WoopsiLinkedListIterator<T>::iterator() {
	return _iterator->data;
}

template <class T>
bool WoopsiLinkedListIterator<T>::moveToNext() {
	if ((_iterator != NULL) && (_iterator->next != NULL)) {
		_iterator = _iterator->next;
		return true;
	}

	return false;
}

template <class T>
bool WoopsiLinkedListIterator<T>::moveToPrevious() {
	if ((_iterator != NULL) && (_iterator->previous != NULL)) {
		_iterator = _iterator->previous;
		return true;
	}

	return false;
}

#endif
