#include "filelistboxdataitem.h"

using namespace WoopsiUI;

FileListBoxDataItem::FileListBoxDataItem(const char* text, const u32 value,
		const u16 normalTextColour,
		const u16 normalBackColour,
		const u16 selectedTextColour,
		const u16 selectedBackColour,
		const bool isDirectory)
			: ListBoxDataItem(text,
						   value,
						   normalTextColour,
						   normalBackColour,
						   selectedTextColour,
						   selectedBackColour) {

	_isDirectory = isDirectory;
}

s8 FileListBoxDataItem::compareTo(const FileListBoxDataItem* item) const {

	// Cast to right type to get to extra fields
	const FileListBoxDataItem* fileItem = (const FileListBoxDataItem*)item;
	
	// Directories come before files
	if (_isDirectory && !fileItem->isDirectory()) return -1;
	if (!_isDirectory && fileItem->isDirectory()) return 1;
	
	// Fall back to standard comparison if both items are of the same type
	return ListBoxDataItem::compareTo(item);
}
