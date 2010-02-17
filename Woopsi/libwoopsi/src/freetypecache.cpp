#include "freetypecache.h"
#include "freetypefacemanager.h"
#include "freetypefont.h"

// I have implemented three different ways to copy the bitmap with anti-
// aliasing.  To switch between them use this define
#define fast

using namespace WoopsiUI;

// A callback function provided by Woopsi. It is used by the cache manager to
// translate a given FTC_FaceID into a new valid FT_Face object, on demand.
static FT_Error faceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface) {
    FT_Error error;
    FaceID* face = (FaceID*)face_id;
    
    // This loads a face from from a file.  It is possible to load a face
	// (faster!) from memory through FT_Open_Face.
    // See here: http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Open_Args
	error = FT_New_Face(library, face->path, face->index, aface);
	if (error) return error;

	// Selects a charmap by it's encoding.  We use Unicode. 
	error = FT_Select_Charmap(*aface, FT_ENCODING_UNICODE);
	if (error) return error;
	
	// If the font's ‘face_index’ is greater than or equal to zero, (*aface)
	// must be non-NULL (or there is a problem...).
	// Computes the index of the previous charmap.  If its rank among the
	// charmaps of the face isn't 0, we have to compute it's index
	if (*aface) face->charMapIndex = ((*aface)->charmap ? FT_Get_Charmap_Index((*aface)->charmap) : 0);
	// This command stores the CharMapIndex in the FontID structure (to speed up
	// glyph loading)... as this function is called on demand, maybe
	// We don't have to initialize that (which feels strange).
	
	// If there was no error, the face should be ready to render glyphs/return metrics now
	return error;
}

FreeTypeCache::FreeTypeCache(u32 cacheBytesSize) {
 	// Calling freetype (libfat must be included and initialized), 

	// Initialize the FreeType library
	_error = FT_Init_FreeType(&_library);
	if (_error) return;

	// We let the cache decide how many max faces and sizes it wil use; don't
	// know what's the point of the pointer that is NULL.
	_error = FTC_Manager_New(_library, 0, 0, cacheBytesSize, &faceRequester, NULL, &_manager); 
	if (_error) return;

	// This structure is responsible for translating the codepoint of a glyph
	// into it's index in a face
	_error = FTC_CMapCache_New(_manager, &_charMapCache);
	if (_error) return;

	// This structure caches very efficiently small bitmaps (whose metrics hold
	// in a char : perfect for the DS)
	_error = FTC_SBitCache_New(_manager, &_sbitCache);
	if (_error) return;
}

FreeTypeCache::~FreeTypeCache() {

	// Free the manager first
	FTC_Manager_Done(_manager);
	
	// Then free the library
	FT_Done_FreeType(_library);
}
    
void FreeTypeCache::loadSbit(FTC_ScalerRec scaler, u32 charMapIndex, u32 codepoint, FTC_SBit * sbit) const{
	// For the flag explanation, see here: 
	// http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_LOAD_TARGET_XXX
	// and here : http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_LOAD_XXX
	u32 index = FTC_CMapCache_Lookup(_charMapCache, scaler.face_id, charMapIndex, codepoint);
	FTC_SBitCache_LookupScaler(_sbitCache, &scaler,  FT_LOAD_RENDER|FT_LOAD_TARGET_NORMAL, index, sbit, NULL);
}
