#ifndef _FREETYPE_OUTLINE_FONT_H_
#define _FREETYPE_OUTLINE_FONT_H_

#include <nds.h>
#include "fontbase.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_H 
#include FT_BITMAP_H

#include "freetypefacemanager.h"
#include "freetypecache.h"
#include "mutablebitmapbase.h"
#include "woopsistring.h"

namespace WoopsiUI {

	class MutableBitmapBase;
	class FreeTypeCache;
     /**
	 * A font class using freetype and outline fonts, in association with a fontcache.
	 * at the moment each glyph is rendered directly on the screen and isn't cached, 
	 * which makes it much slower than a freetypefont
	 * this is an experimental class to test experimental features
	 */
	class FreeTypeOutlineFont : public FontBase {
	public:

		/**
		 * Constructor.
		 * @param fontCache Cache for the font to use.
		 * @param faceId Index of a managed font.
		 * @param width The width of a single glyph.
		 * @param height The height of a single glyph.
		 * @param colour The colour of the font.
		 */
		FreeTypeOutlineFont(FreeTypeCache* fontCache, const FaceId* faceId, const u8 width, const u8 height, const u16 colour);
		
		/**
		 * Destructor.
		 */
		inline ~FreeTypeOutlineFont() {};
		
		/**
		 * Loads a glyph.
		 * @param codepoint the Unicode codepoint of a glyph.
		 */
        void loadGlyph(const u32 codepoint) const {
            _fontCache->loadGlyph(_scaler, _charMapIndex, codepoint, &_glyph);
        }
		/**
		 * Loads the BBbox of  the glyph _glyph.
		 * @param codepoint the Unicode codepoint of a glyph.
		 */
		inline void loadBox() const {     
             FT_Glyph_Get_CBox(_glyph,FT_GLYPH_BBOX_PIXELS,&_bbox);
		 };
		
		 
		/**
		 * Get the width of an individual character.
		 * @param codepoint The character to get the width of.
		 * @return The width of the character in pixels.
		 */
		inline u8 getCharWidth(const u32 codepoint) const {
		    loadGlyph(codepoint);
			return _glyph->advance.x >>16;
		};

		/**
		 * Get the width of a string in pixels when drawn with this font.
		 * @param text The string to check.
		 * @return The width of the string in pixels.
		 */
		inline u16 getStringWidth(const WoopsiString& text) const;
		
		/**
		 * Get the width of a portion of a string in pixels when drawn with this
		 * font.  Useful if you want to determine the length of a section of a
		 * string.
		 * @param text The string to check.
		 * @param startIndex The start point of the substring within the string.
		 * @param length The length of the substring in chars.
		 * @return The width of the substring in pixels.
		 */
		inline u16 getStringWidth(const WoopsiString& text, s32 startIndex, s32 length) const;

		/**
		 * Checks if supplied character is blank in the current font.
		 * @param codepoint The character to check.
		 * @return True if the glyph contains any pixels to be drawn.  False if
		 * the glyph is blank.
		 */
		virtual inline const bool isCharBlank(const u32 codepoint) const { 
		    /*getGlyph(codepoint);
		    FT_BBox box;
            FT_Glyph_Get_CBox(_glyph,FT_GLYPH_BBOX_PIXELS,&box);
			return box.xMax - box.xMin;	
			if ((_sbit->buffer == 0) || (_sbit->width == 0) || (_sbit->height == 0)) return true;*/
			if ((codepoint==10) ||(codepoint==13)) return true;  // FixThis
			return false; 
		};

		/**
		 * Draw an individual character of the font to the specified bitmap.
		 * the upper-left corner is at position (x,y). 
		 * @param bitmap The bitmap to draw to.
		 * @param letter The character to output.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 * @return The x co-ordinate for the next character to be drawn.
		 */
		s16 drawChar(MutableBitmapBase* bitmap, u32 letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);


		/**
		 * Draw an individual character of the font to the specified bitmap 
		 * at the position (x,y) of the pen (i.e. on a baseline)
		 * @param bitmap The bitmap to draw to.
		 * @param letter The character to output.
		 * @param x The x co-ordinate of the pen.
		 * @param y The y co-ordinate of the pen.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 * @return The x co-ordinate for the next character to be drawn.
		 */
		s16 drawBaselineChar(MutableBitmapBase* bitmap, u32 letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
		    return drawChar(bitmap, letter, x, y-getCharTop(letter), clipX1, clipY1, clipX2, clipY2) ;
		};
		
		/**
		 * Get the width of an individual character.
		 * @param letter The character to get the width of.
		 * @return The height of the character in pixels.
		 */
		inline u8 getCharHeight(const u32 codepoint) const { 
		    //loadGlyph(codepoint);
		    //loadBox();
			return _bbox.yMax - _bbox.yMin;
        };     
            
		/**
		 * Get the width of an individual character.
		 * @param letter The character to get the width of.
		 * @return The algebrical distance from baseline to top of the character in pixels.
		 */
		inline s8 getCharTop(const u32 codepoint) const { 
		    //loadGlyph(codepoint);
		    //loadBox();
			return _bbox.yMax;
        }; 
            
		/**
		 * Set the width, height and face (if faceId isn't NULL) of a freetype font.
		 * @param width The width of the new font.
		 * @param height The height of the new font.
		 * @param faceId The faceId of the new font.		 
		 */            
		bool setFont(u8 width, u8 height, const FaceId* faceId = NULL) {
		    if ((!faceId) || (!faceId->isValid())) return false;
		    _scaler.width  = (FT_UInt) width;
			_scaler.height = (FT_UInt) height;	
		    _faceId = faceId;
		    _scaler.face_id = (FTC_FaceID) faceId;
		    return true;
		};
		
	private:
		mutable FreeTypeCache* _fontCache;
		const FaceId* _faceId;
		mutable u32 _charMapIndex;
	    mutable FTC_ScalerRec _scaler;
		mutable FT_Glyph  _glyph;
	    mutable FT_BBox _bbox;
	};
};
#endif
