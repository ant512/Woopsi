#ifndef _FREETYPE_CACHE_H_
#define _FREETYPE_CACHE_H_

#include <nds.h>
#include "fontbase.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_H 
#include FT_BITMAP_H
#include FT_OUTLINE_H

namespace WoopsiUI {

	/**
	 * Container for an FT_Library and 3 FreeType caches FTC_Manager,
	 * FTC_CMapCache and FTC_SBitCache.
	 */
	class FreeTypeCache {
	public:

		/**
		 * Constructor.
		 * @param cacheBytesSize the number of bytes of the freetype font cache.
		 */
		FreeTypeCache(u32 cacheBytesSize = 65536);
		
		/**
		 * Destructor.
		 */
		 ~FreeTypeCache();

		/**
		 * Clear the FreeType font cache.
		 */
		 inline void reset() { FTC_Manager_Reset(_manager); };
		 
		 /**
		 * Loads a small bitmap (with metrics) of a glyph.
		 * @param scaler A scaler of the face (contains the face_id).
		 * @param charMapIndex The index of the charmap of the face.
		 * @param codepoint The Unicode codepoint of a glyph.
		 * @param sbit The sbit that will get the small bitmap.
		 */	
		 void loadSbit(FTC_ScalerRec scaler, u32 charMapIndex, u32 codepoint, FTC_SBit* sbit) const;

		 /**
		 * Loads a glyp (with metrics).
		 * @param scaler A scaler of the face (contains the face_id).
		 * @param charMapIndex The index of the charmap of the face.
		 * @param codepoint The Unicode codepoint of a glyph.
		 * @param sbit The sbit that will get the small bitmap.
		 */			 
		 void loadGlyph(FTC_ScalerRec scaler, u32 charMapIndex, u32 codepoint, FT_Glyph * glyph) const;

		 /**
		 * Renders an outline glyp.
		 * @param scaler A scaler of the face (contains the face_id).
		 * @param charMapIndex The index of the charmap of the face.
		 * @param codepoint The Unicode codepoint of a glyph.
		 * @param sbit The sbit that will get the small bitmap.
		 */			 
		 void renderGlyph(FT_Outline* outline, FT_Raster_Params* params) const {
		     FT_Outline_Render(_library, outline, params);
		 };
		 
	private:
		FT_Library _library;
		FTC_Manager _manager;
		FTC_CMapCache _charMapCache;
		FTC_SBitCache _sbitCache;
		FTC_ImageCache  _imageCache;
		FT_Error _error;
	};
};
#endif
