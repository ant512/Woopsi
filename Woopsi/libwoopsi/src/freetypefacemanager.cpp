#include "freetypefacemanager.h"
#include "mutablebitmapbase.h"
#include "woopsistring.h"
#include "freetypefont.h"
#include "stringiterator.h"

// FreeType assumes that structures allocated by the user and passed as
// arguments are zeroed out except for the actual data. In other words, 
// it is recommended to use ‘calloc’ (or variants of it) instead of ‘malloc’
// for allocation.

using namespace WoopsiUI;

FreeTypeFaceManager::~FreeTypeFaceManager() {
	while (_root != NULL) {
		FreeTypeFaceManagerItem* temp = _root->next;
		free(_root->faceID->path);
		delete _root->faceID;
		delete _root;
		_root = temp;
	};
	_root = NULL;
}

const FaceID* FreeTypeFaceManager::addFace(const char* path, const u8 index) {

	FaceID* faceID = new FaceID;

	// Copy path into faceID struct
	faceID->path = (char*)malloc(strlen(path) + 1);
	strcpy(faceID->path, path);

	faceID->index = index;
	faceID->charMapIndex = 0; // This wil get set later (on demand by the face requester).   

	FreeTypeFaceManagerItem* newItem = new FreeTypeFaceManagerItem;
	newItem->faceID = faceID;
	newItem->next = _root;
	_root = newItem;
	
	return faceID;
}
