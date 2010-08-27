#ifndef _FREETYPE_FACE_MANAGER_H_
#define _FREETYPE_FACE_MANAGER_H_

#include <nds.h>
#include "fontbase.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_H 
#include FT_BITMAP_H

namespace WoopsiUI {

	/** 
	* FontTYpe  descibes the type of the fonts supported by the facedispatcher/freetypefontmanager
	 * It was introduced to enable fast switching of fonts
	 */
    enum FaceType {  // for speed 
        INVALID_FACE,                    /**< invalid font */
        PACKED_FACE,			          /**< packed font */
        FILE_FREETYPE_FACE,		  /**< freetype font accessible through fat */
        MEMORY_FREETYPE_FACE   /**< freetype font loaded in memory */
    }; 
    
    
struct PackedFaceId  {
            FontBase* fontBase;
};

struct MemoryFreeTypeFaceId {
            const unsigned char * address; 
            u32 size;
            u8 index;
            u8 nMap;
            u32 charMapIndex;
};   
    
struct FileFreeTypeFaceId{                    
            const char* path; 
            u8 index;
            u8 nMap;
            u32 charMapIndex;
};
    
union FaceIdUnion{
                PackedFaceId packed;
                MemoryFreeTypeFaceId memory;
                FileFreeTypeFaceId file;              
};
	/** 
	 * FaceID is a Woopsi structure identifying a face.
	 * @param path Pointer to the path of the font.(if size is 0) and to the place in memory where the font is  (if size>0), NULL if the font is corrupted. 
	 * @param size is the nb of byte of the font if it is in memory and 0 if the font is on disk
	 * @param index Index of the face in the font (0 is the default....beware,
	 * if you ask for an index that is not in the font it will mess things up
	 * (chars might not display, I experienced this)).
	 * @param charMapIndex caches the charMapIndex value (we are using Unicode)
	 * so it doesn't have to be computed each time.
	 */
	 // FaceID's charMapIndex needs a freefont fo be initialised and before it is initialised, we don't know if the FaceID is valid or not when you add it to a FreeTypeFaceManager, but later on when you instanciate a FreeTypeFont with the FaceID 
	class FaceId {
	    
        public:
            FaceId(const char * path, u8 index) {
                _type = FILE_FREETYPE_FACE;
                _face.file.path = path;
                _face.file.index  = index;
                _face.file.charMapIndex = 0;
                _face.file.nMap = 0;
            };	    
            FaceId(const u8 * address, u32 size, u8 index) {
                _type = MEMORY_FREETYPE_FACE;       
                _face.memory.address = address;
                _face.memory.size = size;
                _face.memory.index  = index;
                _face.memory.charMapIndex = 0;
                _face.memory.nMap = 0; 
            };
            FaceId(FontBase * fontBase) {
                _type = PACKED_FACE;
                _face.packed.fontBase = fontBase;
            };       
             inline bool isValid() const {return (_type != INVALID_FACE);};
             
             inline void declareInvalid() {_type = INVALID_FACE; };
             
             FaceIdUnion face() const {return _face;};
             
             FaceType type() const {return _type;};
             
             void setCharMapIndex(u32 charMapIndex) {
                 switch(_type){
                 case MEMORY_FREETYPE_FACE:
                     _face.memory.charMapIndex = charMapIndex;
                     return;
                 case FILE_FREETYPE_FACE:
                     _face.file.charMapIndex = charMapIndex;
                     return;  
                 case INVALID_FACE:
                 case PACKED_FACE:
                     declareInvalid();
                     return;
                 }
             };            
            void setNMap(u8 nMap) { 
                if (!nMap) {
                    declareInvalid();
                    return;
                }
                 switch(_type){
                 case MEMORY_FREETYPE_FACE:
                     _face.memory.nMap = nMap;
                     return;
                  case FILE_FREETYPE_FACE:
                     _face.file.nMap = nMap;
                     return;  
                 case INVALID_FACE:
                 case PACKED_FACE:
                     declareInvalid();
                     return;
                 }                
            };
            u8 nMap() const {
               switch(_type){
                 case MEMORY_FREETYPE_FACE:
                     return _face.memory.nMap;
                  case FILE_FREETYPE_FACE:
                     return _face.file.nMap;  
                 case INVALID_FACE:
                 case PACKED_FACE:
                     return 0;            
               }
            };
               
        private:
            FaceType _type;
            FaceIdUnion _face;
	};

	/** 
	 * FaceManager is a basic linked list containing the installed fonts.  You
	 * are supposed to add all the fonts that you are going to use.  You are not
	 * supposed to remove fonts once you have added them.  FreeType expects this
	 * data to be persistent and not to change (if you really need to change it,
	 * it's possible but you have to tell FreeType that you did).
	 */
	struct FreeTypeFaceManagerItem {
		FaceId* faceId;
		FreeTypeFaceManagerItem *next;
	};
   
	class FreeTypeFaceManager {
	public: 
		/**
		 * Constructor.
		 */
		 FreeTypeFaceManager() { _root = NULL; };
		 
		/**
		 * Stores a FaceId inside the dynamic list and returns it's address.
		 * @param faceId A FaceId too add to store inn the FreeTypeFaceManager.
		 * @return The faceId.
		 */     		 
		 const FaceId* push(FaceId* faceId);

		/**
		 * Associates a packed font to a FaceId in the freetypeFaceManager and returns it.
		 * @param fontBase The fontBase of the packed font.
		 * @return The FaceId of the packed font.
		 */    		 
		 const FaceId* addFace(FontBase* fontBase) {
		     return push(new FaceId(fontBase));
		 };
		 
		/**
		 * Associates a freetype font (with index=0) in a file to a FaceId in the freetypeFaceManager and returns it.
		 * @param path Pointer to the path of the font.
		 * @param index Index of the face in the fonts as it may have many (default is 0).
		 * @return The FaceId of the freetype font.
		 */           
		 const FaceId* addFace(const char* path, const u8 index = 0) {
		     return push(new FaceId( path, index));
		 };
		 
		/**
		 * Associates a freetype font (with index=0) in memory to a FaceId in the freetypeFaceManager and returns it.
         * @param data pointer to the freetype font in memory
         * @param size size of the font in memory          
         * @return The FaceID of the font
		 */           
		 const FaceId* addFace(const u8* data, u32 size, const u8 index = 0) {
		     return push(new FaceId(data, size, index));
		 };	
		 
		 
		/**
		 * Destructor.
		 */
		~FreeTypeFaceManager();  // Beware this destroys FaceIDs, that may be needed by freetype 

	private:
		FreeTypeFaceManagerItem* _root;		/**< Internal stack of data items. */
	};
}

#endif
