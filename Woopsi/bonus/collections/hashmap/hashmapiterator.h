#ifndef _HASHMAP_ITERATOR_H_
#define _HASHMAP_ITERATOR_H_

#include "hashmap.h"

/**
 * Class providing sequential access to a hashmap.  Rather than force
 * the coder to work with the raw list data, this class abstracts it away
 * with ADO RecordSet-style functionality.
 *
 * This class cannot be instantiated; rather, an instance of the class
 * is created by calling getIterator() on an instance of the HashMap.
 */
template <class T>
class HashMapIterator {
public:

	/**
	 * Destructor.
	 */
	~HashMapIterator();

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
	friend class HashMap<T>;

	HashMap<T>* _hashmap;
	HashMapBucket<T>* _bucket;
	s32 _index;

	/**
	 * Constructor is private to prevent creation outside of the hashmap
	 */
	HashMapIterator(HashMap<T>* list);
};

template <class T>
HashMapIterator<T>::HashMapIterator(HashMap<T>* hashmap) {
	_hashmap = hashmap;
	_index = 0;
	_bucket = NULL;

	moveToFirst();
}

template <class T>
HashMapIterator<T>::~HashMapIterator() {
}

template <class T>
void HashMapIterator<T>::moveToFirst() {

	// Locate the first populated index
	for (s32 i = 0; i < _hashmap->reservedSize();  i++) {
		if (_hashmap->_data[i] != NULL) {
			_index = i;
			_bucket = _hashmap->_data[i];
			return;
		}
	}
}

template <class T>
void HashMapIterator<T>::moveToLast() {
	
	// Locate the last populated index
	s32 j = _hashmap->reservedSize() - 1;
	for (s32 i = 0; i < _hashmap->reservedSize(); i++) {
		if (_hashmap->_data[j - i] != NULL) {
			_index = j - i;

			// Locate the last item in the list
			_bucket = _hashmap->_data[j - i];

			while (_bucket->next != NULL) {
				_bucket = _bucket->next;
			}

			return;
		}
	}
}

template <class T>
T& HashMapIterator<T>::iterator() {
	return _bucket->data;
}

template <class T>
bool HashMapIterator<T>::moveToNext() {

	// Do we have an item in the bucket?
	if (_bucket == NULL) {
		// No item
		return false;
	}

	// Already got an item in the bucket; are there other buckets in the list?
	if (_bucket->next != NULL) {

		// Move to next bucket in list
		_bucket = _bucket->next;
		return true;
	}

	// No more buckets in the list; try to move to new slot
	for (s32 i = _index + 1; i < _hashmap->reservedSize(); i++) {
		if (_hashmap->_data[i] != NULL) {
			_bucket = _hashmap->_data[i];
			_index = i;
			return true;
		}
	}

	// No more data; reset values
	_index = 0;
	_bucket = NULL;
	return false;
}

template <class T>
bool HashMapIterator<T>::moveToPrevious() {

	// Do we have an item in the bucket?
	if (_bucket == NULL) {
		// No item
		return false;
	}

	// Already got an item in the bucket; are there other buckets in the list?
	if (_bucket->previous != NULL) {

		// Move to previous bucket in list
		_bucket = _bucket->previous;
		return true;
	}

	// No more buckets in the list; try to move to new slot
	s32 j = _index - 1;

	for (s32 i = 0; i < _index - 1; i++) {
		if (_hashmap->_data[j - i] != NULL) {
			_bucket = _hashmap->data[j - i];
			_index = j - i;
			return true;
		}
	}

	// No more data; reset values
	_index = 0;
	_bucket = NULL;
	return false;
}

#endif
