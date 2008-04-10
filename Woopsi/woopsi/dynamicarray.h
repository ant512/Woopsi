#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

#include <nds.h>

#define DYNAMIC_ARRAY_SIZE 100

/**
 * Class providing a dynamic array; that is, an array that will automatically
 * grow to accommodate new data.  It provides a fast way to randomly access
 * a list of data.  Essentially, it provides the most important functionality
 * of the STL vector class without any of the overhead of including an STL
 * class.
 *
 * If the data to be stored will store a lot of data that will predominantly
 * be read sequentially, consider using the LinkedList class instead.  Resizing
 * the list is an expensive operation that will occur frequently when filling
 * the array with large amounts of data.  Adding new data to the linked list is
 * very inexpensive.
 */
template <class T>
class DynamicArray {
public:

	/**
	 * Constructor.
	 */
	inline DynamicArray();

	/**
	 * Destructor.
	 */
	inline ~DynamicArray();

	/**
	 * Get the size of the array.
	 * @return The size of the array.
	 */
	inline const s32 size() const;

	/**
	 * Add a value to the end of the array.
	 * @param value The value to add to the array.
	 */
	void push_back(const T &value);

	/**
	 * Insert a value into the array.
	 * @param index The index to insert into.
	 * @param value The value to insert.
	 */
	void insert(const s32 index, const T &value);

	/**
	 * Remove the last element from the array.
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
	 * Check if the array has any data.
	 * @return True if the array is empty.
	 */
	inline bool empty() const;

	/**
	 * Remove all data.
	 */
	void clear();

	/**
	 * Overload the [] operator to allow array-style access.
	 * @param index The index to retrieve.
	 * @return The value at the specified index.
	 */
	T& operator[](const s32 index) const;

	/**
	 * Return the starting index.  Beginning element is always 0.
	 * Only included to allow painless swapping between vector and
	 * this class.
	 * @return Always returns 0.
	 */
	u32 begin() const;

private:
	T* _data;								/**< Internal array of data items */
	s32 _size;								/**< Number of items in the array */
	s32 _reservedSize;						/**< Total size of the array including unpopulated slots */

	/**
	 * Resize the array if it is full.
	 */
	void resize();
};

template <class T>
DynamicArray<T>::DynamicArray() {
	_size = 0;
	_reservedSize = DYNAMIC_ARRAY_SIZE;
	_data = new T[_reservedSize];
}

template <class T>
DynamicArray<T>::~DynamicArray() {
	delete [] _data;
}

template <class T>
const s32 DynamicArray<T>::size() const {
	return _size;
}

template <class T>
void DynamicArray<T>::push_back(const T &value) {

	// Ensure the array is large enough to contain this data
	resize();

	// Add data to array
	_data[_size] = value;

	// Remember we've filled a slot
	_size++;
}

template <class T>
void DynamicArray<T>::pop_back() {
	if (_size >= 1) {
		// We can just reduce the used size of the array, as the value
		// will get overwritten automatically
		_size--;
	}
}

template <class T>
void DynamicArray<T>::insert(const s32 index, const T &value) {

	// Bounds check
	if ((index >= _size) || (_size == 0)) {
		push_back(value);
		return;
	}

	// Ensure the array is large enough to contain this data
	resize();

	// Shift all of the data back one place to make a space for the new data
	for (s32 i = _size; i > index; i--) {
		_data[i] = _data[i - 1];
	}

	// Add data to array
	_data[index] = value;

	// Remember we've filled a slot
	_size++;
}

template <class T>
void DynamicArray<T>::erase(const s32 index) {

	// Bounds check
	if (index >= _size) return;

	// Shift all of the data back one place and overwrite the value
	for (s32 i = index; i < _size - 1; i++) {
		_data[i] = _data[i + 1];
	}

	// Remember we've removed a slot
	_size--;
}

template <class T>
void DynamicArray<T>::resize() {
	// Do we need to redim the array?
	if (_reservedSize == _size) {
		
		// We have filled the array, so resize it

		// Create new array
		u32 newSize = _reservedSize + DYNAMIC_ARRAY_SIZE;
		T* newData = new T[newSize];

		// Copy old array to new
		for (s32 i = 0; i < _reservedSize; i++) {
			newData[i] = _data[i];
		}

		//memcpy(newData, _data, sizeof(T) * _reservedSize);

		// Delete the old array
		delete [] _data;

		// Update values
		_data = newData;
		_reservedSize = newSize;
	}
}

template <class T>
T& DynamicArray<T>::at(const s32 index) const {
	return _data[index];
}

template <class T>
bool DynamicArray<T>::empty() const {
	return (_size == 0);
}

template <class T>
T& DynamicArray<T>::operator[](const s32 index) const {
	return _data[index];
}

template <class T>
void DynamicArray<T>::clear() {
	// All we need to do is reset the size value
	_size = 0;
}

template <class T>
u32 DynamicArray<T>::begin() const {
	return 0;
}

#endif
