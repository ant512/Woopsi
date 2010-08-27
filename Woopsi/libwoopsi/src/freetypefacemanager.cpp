#include "freetypefacemanager.h"
//#include "mutablebitmapbase.h"
//#include "woopsistring.h"
//#include "freetypefont.h"
//#include "stringiterator.h"

// FreeType assumes that structures allocated by the user and passed as
// arguments are zeroed out except for the actual data. In other words, 
// it is recommended to use ‘calloc’ (or variants of it) instead of ‘malloc’
// for allocation.

using namespace WoopsiUI;

FreeTypeFaceManager::~FreeTypeFaceManager() {
	while (_root != NULL) {
		FreeTypeFaceManagerItem* temp = _root->next;
		delete _root->faceId;
		delete _root;
		_root = temp;
	};
	_root = NULL;
}

const FaceId* FreeTypeFaceManager::push(FaceId* faceId) {
	FreeTypeFaceManagerItem* newItem = new FreeTypeFaceManagerItem;
	newItem->faceId = faceId;
	newItem->next = _root;
	_root = newItem;
	return faceId;
}

