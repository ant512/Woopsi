#ifndef _HASHMAP_H_
#define _HASHMAP_H_

// Hashmap table size *must* be a power of two as it is used as a bitmask in the hash function
const s32 HASHMAP_DEFAULT_SIZE = 32;

template <class T>
class HashMapIterator;

/**
 * Struct representing a single item in a hash map bucket.
 */
template <class T>
struct HashMapBucket {
	T data;
	const char* key;
	HashMapBucket<T>* next;
	HashMapBucket<T>* previous;
};

/**
 * Class providing a hashmap.  A hashmap is a collection data structure storing key/value pairs.
 * This implementation uses chaining, or linked lists of buckets, to handle hash collisions.
 * The hashmap does not resize to contain more data.  As more data is added, the linked lists
 * will automatically grow.  This means that the hashtable will gradually degrade in performance
 * as it fills up.
 * Uses Bob Jenkins' one-at-a-time hash function from Wikipedia.
 */
template <class T>
class HashMap {
public:
	/**
	 * Constructor.
	 */
	HashMap();

	/**
	 * Destructor.
	 */
	~HashMap();

	/**
	 * Get the number of items in the hashmap.
	 * @return The number of items in the hashmap.
	 */
	const s32 size() const;

	/**
	 * Add a key/value pair into the hashmap.
	 * @param key The unique key for this value.
	 * @param value The value to add to the hashmap.
	 */
	void add(const char* key, const T &value);

	/**
	 * Get the value at the specified key.
	 * @param key The key of the item to return.
	 */
	T item(const char* key);

	/**
	 * Erase a single value at the specified key.
	 * @param key The key of the item to erase.
	 */
	void erase(const char* key);

	/**
	 * Check if the map has any data.
	 * @return True if the map is empty.
	 */
	bool empty();

	/**
	 * Remove all data.
	 */
	void clear();

	/**
	 * Get an iterator for this hashmap.
	 * @return An iterator for the hashmap.
	 */
	HashMapIterator<T>* getIterator();

private:
	friend class HashMapIterator<T>;

	HashMapBucket<T>** _data;
	s32 _size;
	s32 _reservedSize;

	/**
	 * Get the hash of the supplied key.
	 * @param key The key to find the hash of.
	 * @return The hash of the key
	 */
	const s32 getHash(const char* key) const;

	/**
	 * Get the size of the array.
	 * @return The size of the array.
	 */
	const s32 reservedSize() const;
};

template <class T>
HashMap<T>::HashMap() {
	_reservedSize = HASHMAP_DEFAULT_SIZE;
	_data = new HashMapBucket<T>*[_reservedSize];
	_size = 0;

	// Initialise the array
	for (s32 i = 0; i < _reservedSize; i++) {
		_data[i] = NULL;
	}
}

template <class T>
HashMap<T>::~HashMap() {

	clear();

	// Delete main array
	delete [] _data;
}

template <class T>
const s32 HashMap<T>::size() const {
	return _size;
}

template <class T>
const s32 HashMap<T>::reservedSize() const {
	return _reservedSize;
}

template <class T>
void HashMap<T>::add(const char* key, const T &value) {

	// Get hash for this key
	s32 hash = getHash(key);

	// Create bucket
	HashMapBucket<T>* bucket = new HashMapBucket<T>;
	bucket->data = value;
	bucket->key = key;
	bucket->previous = NULL;
	bucket->next = NULL;

	// Remember we've added a new item
	_size++;

	// Attempt to insert into empty bucket
	if (_data[hash] == NULL) {
		_data[hash] = bucket;
		return;
	}

	// Insert bucket into start of list
	bucket->next = _data[hash];
	_data[hash]->previous = bucket;
	_data[hash] = bucket;
}

template <class T>
T HashMap<T>::item(const char* key) {

	// Get hash for this key
	s32 hash = getHash(key);

	// Locate item
	HashMapBucket<T>* bucket = _data[hash];

	// Have we found the item?
	while ((bucket->next != NULL) && (strcmp(bucket->key, key))) {
		bucket = bucket->next;
	}

	return bucket->data;
}

template <class T>
void HashMap<T>::erase(const char* key) {

	// Get hash for this key
	s32 hash = getHash(key);

	// Locate item
	HashMapBucket<T>* bucket = _data[hash];

	// Have we found the item?
	while ((bucket->next != NULL) && (strcmp(bucket->key, key))) {
		bucket = bucket->next;
	}

	// Is the item the only one in its slot?
	if ((bucket->previous == NULL) && (bucket->next == NULL)) {

		// Only item in the slot, so just kill it
		delete _data[hash];
		_data[hash] = NULL;
		_size--;

		return;
	}

	// Is the item the first one in the list?
	if ((bucket->previous == NULL) && (bucket->next != NULL)) {

		// Item is first in list
		_data[hash] = bucket->next;
		bucket->next->previous = NULL;
		
		// Delete the item
		delete bucket;
		_size--;

		return;
	}

	// Is the item the last one in the list?
	if (bucket->next == NULL) {

		// Item last in list
		bucket->previous->next = NULL;

		// Delete the item
		delete bucket;
		_size--;

		return;
	}
		
	// Remove item from the middle of the list
	bucket->next->previous = bucket->previous;
	bucket->previous->next = bucket->next;
	delete bucket;
	_size--;
}

template <class T>
void HashMap<T>::clear() {

	// Loop through all array slots deleting all buckets
	for (s32 i = 0; i < _reservedSize; i++) {
		HashMapBucket<T>* bucket = _data[i];
		HashMapBucket<T>* nextBucket = NULL;

		// Delete all buckets in this slot
		while (bucket != NULL) {
			nextBucket = bucket->next;

			delete bucket;

			bucket = nextBucket;
		}

		_data[i] = NULL;
	}

	_size = 0;
}

template <class T>
bool HashMap<T>::empty() {
	return (_size == 0);
}

template <class T>
const s32 HashMap<T>::getHash(const char* key) const {
    s32 hash = 0;
	char ch;
     
    while(*key) {
		ch = *key++;

        hash += ch;
        hash += (ch << 10);
        hash ^= (ch >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash & (_reservedSize - 1);
}

template <class T>
HashMapIterator<T>* HashMap<T>::getIterator() {
	return new HashMapIterator<T>(this);
}

#endif
