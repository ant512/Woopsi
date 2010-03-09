#ifndef _FREETYPE_FONT_H_
#define _FREETYPE_FONT_H_

#include <nds.h>
#include "fontbase.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_H 
#include FT_BITMAP_H

#include "freetypefacemanager.h"

namespace WoopsiUI {

	class MutableBitmapBase;
	class FreeTypeCache;

	/**
	 * A font class using freetype, in association with a fontcache.
	 */
	class FreeTypeFont : public FontBase {
	public:

		/**
		 * Constructor.
		 * @param fontCache Cache for the font to use.
		 * @param faceID Index of a managed font.
		 * @param width The width of a single glyph.
		 * @param height The height of a single glyph.
		 * @param colour The colour of the font.
		 */
		FreeTypeFont(const FreeTypeCache* fontCache, const FaceID* faceID, const u8 width, const u8 height, const u16 colour);
		
		/**
		 * Destructor.
		 */
		inline ~FreeTypeFont() {};
		
		/**
		 * Loads a small bitmap (with metrics) of a glyph.
		 * @param codepoint the Unicode codepoint of a glyph.
		 */
		void getSbit(const u32 codepoint) const;
		
		/**
		 * Get the width of an individual character.
		 * @param codepoint The character to get the width of.
		 * @return The width of the character in pixels.
		 */
		inline u16 getCharWidth(const u32 codepoint) const { 
			getSbit(codepoint);
			return (u16) _sbit->xadvance;
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
			getSbit(codepoint);
			if ((_sbit->buffer == 0) || (_sbit->width == 0) || (_sbit->height == 0)) return true;
			return false; 
		};

		/**
		 * Draw an individual character of the font to the specified bitmap.
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

	private:
		const FreeTypeCache* _fontCache;
		const FaceID* _faceID;
		u32 _charMapIndex;
		FTC_ScalerRec _scaler;
		FTC_SBit _sbit;
	};
}

#endif
