#include "freetypecache.h"
#include "freetypefacemanager.h"
//#include "freetypefont.h"

using namespace WoopsiUI;

// A callback function provided by Woopsi. It is used by the cache manager to
// translate a given FTC_FaceID into a new valid FT_Face object, on demand.
static FT_Error faceRequester(FTC_FaceID face_id, FT_Library library, FT_Pointer request_data, FT_Face* aface) {
    FaceId* faceId = (FaceId*) face_id;
    FaceType type = faceId->type();
    FaceIdUnion face = faceId->face();
    FT_Error error;   
    /*
 
    if (!face->isValid()) return -666; // shouldn't be usefull as invalid freefont shouldn't call the faceRequester
    */
    
    // This loads a face from from a file (if face->size is 0) and from memory otherwise.  
    // See here: http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#FT_Open_Args
    
	if (type == FILE_FREETYPE_FACE) error = FT_New_Face(library, face.file.path, face.file.index, aface);
	else error = FT_New_Memory_Face(library, face.memory.address, face.memory.size, face.memory.index, aface);
	if (error) {
	    faceId->declareInvalid();
	    return error;
	}
	//  if the font has a unicode charmap, it'll be selected. In other cases (old/asian fonts) we have to fall back to some other charmap (the first one we find, basically)
	if ((!aface) || (!*aface) || (!((*aface)->num_charmaps))) return -666;
	if (!(*aface)->charmap) {// no unicode charmap	
         for (int n = 0; n < (*aface)->num_charmaps; n++) {
             error = FT_Set_Charmap(*aface, (*aface)->charmaps[n]);
             if (!error) break;  // success
         }
        if (!(*aface)->charmap) { // could not select a char map 
            faceId->declareInvalid();
            return -666;
        } 
        faceId->setNMap((*aface)->num_charmaps);
	}

	/*// Selects a charmap by it's encoding.  We use Unicode. 
	error = FT_Select_Charmap(*aface, FT_ENCODING_UNICODE);
	if (error) return error;
*/
	
	// If the font's ‘face_index’ is greater than or equal to zero, (*aface)
	// must be non-NULL (or there is a problem...).
	// Computes the index of the previous charmap.  If its rank among the
	// charmaps of the face isn't 0, we have to compute it's index

	faceId->setCharMapIndex(((*aface)->charmap ? FT_Get_Charmap_Index((*aface)->charmap): 0));
	// This command stores the CharMapIndex in the FontID structure (to speed up
	// glyph loading)... as this function is called on demand, maybe
	// We don't have to initialize that (which feels strange).
	
	return 0;
};

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
	
	// This structure is designed to hold many distinct glyph images while not exceeding a certain memory threshold.
	 _error = FTC_ImageCache_New(_manager, &_imageCache);
	 if (_error) return;
};

FreeTypeCache::~FreeTypeCache() {

	FTC_Manager_Done(_manager);
	FT_Done_FreeType(_library);
};
    
void FreeTypeCache::loadSbit(FTC_ScalerRec scaler, u32 charMapIndex, u32 codepoint, FTC_SBit * sbit) const{
	u32 index = FTC_CMapCache_Lookup(_charMapCache, scaler.face_id, charMapIndex, codepoint);
	FTC_SBitCache_LookupScaler(_sbitCache, &scaler,  FT_LOAD_RENDER|FT_LOAD_TARGET_NORMAL, index, sbit, NULL);
};

void FreeTypeCache::loadGlyph(FTC_ScalerRec scaler, u32 charMapIndex, u32 codepoint, FT_Glyph * glyph) const {
	u32 index = FTC_CMapCache_Lookup(_charMapCache, scaler.face_id, charMapIndex, codepoint);
	FTC_ImageCache_LookupScaler(_imageCache, &scaler,  FT_LOAD_NO_BITMAP, index, glyph, NULL);//FT_LOAD_TARGET_NORMAL
};
