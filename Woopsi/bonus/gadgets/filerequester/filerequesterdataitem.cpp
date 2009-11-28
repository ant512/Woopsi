#include "filerequesterdataitem.h"

using namespace WoopsiUI;

FileRequesterDataItem::FileRequesterDataItem(const char* text, const u32 value,
		const u16 normalTextColour,
		const u16 normalBackColour,
		const u16 selectedTextColour,
		const u16 selectedBackColour,
		const bool isDirectory)
			: ListDataItem(text,
						   value,
						   normalTextColour,
						   normalBackColour,
						   selectedTextColour,
						   selectedBackColour) {

	_isDirectory = isDirectory;
}

s8 FileRequesterDataItem::compareTo(const ListDataItem* item) const {

	// Cast to right type to get to extra fields
	FileRequesterDataItem* fileItem = (FileRequesterDataItem*)item;
	
	// Directories come before files
	if (_isDirectory && !fileItem->isDirectory()) return -1;
	if (!_isDirectory && fileItem->isDirectory()) return 1;
	
	return strcmp(getText(), fileItem->getText());
}
