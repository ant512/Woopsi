#include "freetypefont.h"
#include "woopsistring.h"
#include "stringiterator.h"

using namespace WoopsiUI;

FreeTypeFont::FreeTypeFont(FreeTypeCache* fontCache, const FaceId* faceId, const u8 width, const u8 height, const u16 colour) : FontBase(height, 0) { // Don't need transparency

	setColour(colour);
	_fontCache = fontCache;
	_faceId = faceId;
	_charMapIndex = 0;
	_scaler.face_id = (FTC_FaceID) faceId;
	_scaler.width = (FT_UInt) width;
	_scaler.height = (FT_UInt) height;
	_scaler.pixel = 110; // DPI
	_scaler.x_res = 0;  // Computations are done in pixels and not real units
	_scaler.y_res = 0;  // Computations are done in pixels and not real unit
	getSbit(' '); 		// Load the ' ' blank char...no bitmap, no drawing, but
						// metrics, fastest loading
}

void FreeTypeFont::getSbit(const u32 codepoint) const {
    _fontCache->loadSbit(_scaler, _charMapIndex, codepoint, (FTC_SBit *) &_sbit);
    if (!_faceId->nMap() ||(_sbit->xadvance)) return;
    u8 tempCMap =  _charMapIndex;
    do {
        tempCMap ++;
        if (tempCMap >= _faceId->nMap()) tempCMap  = 0;
        if (tempCMap == _charMapIndex) return;
         _fontCache->loadSbit(_scaler, tempCMap , codepoint, (FTC_SBit *) &_sbit);
         
        } while (!_sbit->xadvance);
}

u16 FreeTypeFont::getStringWidth(const WoopsiString& text) const {     	
	if ((!_faceId->isValid())|| !text.getLength()) return 0;
	
	u32 width = 0; 
	StringIterator* iterator = text.newStringIterator();
	
	do { 
		u32 codepoint = iterator->getCodePoint();
		
		if ((codepoint != 10) && (codepoint != 13)) {         
			getSbit(codepoint);
			width += _sbit->xadvance;
		}
	}  while (iterator->moveToNext());
	
	delete iterator;
	return width;
}

u16 FreeTypeFont::getStringWidth(const WoopsiString& text, s32 startIndex, s32 length) const { 
	if ((!_faceId->isValid())|| !length) return 0;
	StringIterator* iterator = text.newStringIterator();
	if (!iterator->moveTo(startIndex)) {
		delete iterator;
		return 0;
	}
		
	u32 width = 0; 
	do {
		u32 codepoint = iterator->getCodePoint();
		if ((codepoint != 10) && (codepoint != 13)) {         
			getSbit(codepoint);
			width += _sbit->xadvance;
		}
		length--;
	} while ((iterator->moveToNext()) && (length > 0));
	
	delete iterator;
	return width;
};

/**
The following array has been produced with the following Maple code : 

for a from 0 to 255 do
     L[a]:=0:
     for b from 0 to 31 do
 n:=(floor(a*b/255)-floor(a*b/256)):
         if (n>0) then L[a] := L[a] + 2^b; fi;
     od;
 od:
 seq(sprintf("0x%X%X",ceil(L[a]/2^16),modp(L[a],2^16)), a=0..255);

      
The purpose of this array is that:
	if 0<=a<=255 and 0<=b<= 31 then a*b/255 = ((a*b) << 6) + ((transparencyJump[a] & Bit(b)) ? 1:0); 

The aim of this array is to replace a possibly costly division by 255 by a bit
rotation and an addition.  The gain in speed depends on the arm assembly.
           
It could be made even faster with an array of 256*32*1 byte = 8k, by replacing
the multiplication and the division by an array lookup.

by a*b/255 = result[a<<5+b]
*/


 u32 transparencyJump[256] =  {
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 
0x0, 0x40008000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x0, 
0x0, 0x80000, 0x40008000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20000, 0x20, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x42108420, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x400, 0x4002000, 0x20, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x80000, 0x0, 0x40008000, 0x0, 0x0, 0x40, 0x0, 0x10204080, 0x0, 0x20, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x10000, 0x0, 0x0, 0x49249248, 0x0, 0x0, 0x20000, 0x800, 0x20, 0x0, 0x2000, 0x40800, 0x80, 0x0, 
0x0, 0x20000, 0x0, 0x80000, 0x800, 0x0, 0x42108420, 0x0, 0x8000, 0x20, 0x0, 0x80000, 0x4000, 0x0, 0x0, 0x800, 
0x0, 0x0, 0x0, 0x100, 0x40800, 0x1000, 0x4002000, 0x40008000, 0x20, 0x80, 0x800, 0x8000, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0xA0000, 0x2000, 0x200, 0x20, 0x40008000, 0x10000, 0x0, 0x0, 0x80000, 0x20000, 0x804200, 0x2000, 
0x0, 0x40000, 0x10204080, 0x0, 0x80, 0x1001000, 0x20020, 0x400, 0x0, 0x42108420, 0x0, 0x10000, 0x40, 0x4002000, 0x200, 0x0, 
0x0, 0x8000, 0x40000, 0x0, 0x10004000, 0x80020, 0x100, 0x4000, 0x0, 0x0, 0x49249248, 0x0, 0x0, 0x80000, 0x2400, 0x80, 
0x20000, 0x4002000, 0x800, 0x4010400, 0x8020, 0x81000, 0x0, 0x0, 0x2000, 0x20000, 0x40800, 0x44008000, 0x80, 0x8800, 0x0, 0x0, 
0x0, 0x0, 0x22000, 0x220, 0x40000, 0x0, 0x80000, 0x804200, 0x800, 0x10004000, 0x1080, 0x20000, 0x42108420, 0x0, 0x84000, 0x200, 
0x8000, 0x40800, 0x10020, 0x800, 0x0, 0x41041040, 0x80000, 0x2080, 0x4002000, 0x100, 0x8000, 0x10204080, 0x20000, 0x40408000, 0x800, 0x0, 
0x0, 0x2020, 0x4000, 0x804200, 0x10000, 0x20080, 0x4010400, 0x80200, 0x40800, 0x800, 0x1001000, 0x2000, 0x4002000, 0x18004000, 0x60008000, 0x80000, 
0x20, 0x40, 0x80, 0x300, 0xC00, 0x3000, 0x1C000, 0xE0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFFFFFFFE};

s16 FreeTypeFont::drawBaselineChar(MutableBitmapBase* bitmap, u32 letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
    if (!_faceId->isValid()) return x;
    
	// No need to draw blank or glyphs missing a bitmap
	switch (letter) {
		case 10:  // Line feed
		case 13:  // Carriage return
			return x;
		default:
			break;
	}
	
	getSbit(letter);

	u8 fontWidth = _sbit->width;
	u8 fontHeight = _sbit->height ;
	s16 output = x +  _sbit->xadvance;
	if (!_sbit->buffer) return output;

	u8 maxGrays = _sbit->max_grays;
	if (maxGrays == 0) return output;
	// offsets for the glyph
	x += _sbit->left;
	y -= _sbit->top;

	
	// This should be done with the new Rect class, but let's get it working first
	// TODO (olivier.binda@wanadoo.fr)
	
	// no need to blit to screen
	if ((clipX1>clipX2) || (clipY1>clipY2)) return output;
	if (y > clipY2) return output;
	if (y + fontHeight < clipY1) return output;
	if (x > clipX2) return output;
	if (x + fontWidth < clipX1) return output;

	// Calculate values for clipping
	s16 startX = x > clipX1 ? x : clipX1;
	s16 endX = x + fontWidth - 1 < clipX2 ? x + fontWidth - 1: clipX2;
	s16 startY = y > clipY1 ? y : clipY1;
	s16 endY = y + fontHeight - 1 < clipY2 ? y + fontHeight - 1: clipY2;

	// Calculate clipping starting offsets - tells us the pixel offset
	// from the top-left of the character that we start drawing from
	u16 offsetStartX = x > clipX1 ? 0 : clipX1 - x;
	u16 offsetStartY = y > clipY1 ? 0 : clipY1 - y;

	// Calculate dimensions of region to copy
	u16 clipWidth = (endX - startX) + 1;
	u16 clipHeight = (endY - startY) + 1;

	// Ensure region to be drawn does not exceed the size of the character
	if (clipWidth > fontWidth - offsetStartX) clipWidth = fontWidth - offsetStartX;
	if (clipHeight > fontHeight - offsetStartY) clipHeight = fontHeight - offsetStartY;

	// Abort if there is no copying to be done
	if ((clipWidth == 0) || (clipHeight == 0)) return output;

	// Copy the pixel data
	u8 grayLevel;
	u8* src = _sbit->buffer;
	u16 rgb;
	u16 bitmapColour;
	
	// This is the colour of the font
	u16 sourceColour = getColour();
	u16 r = sourceColour & 0x7C00;
	u16 g = sourceColour & 0x03E0;
	u16 b = sourceColour & 0x001F;
	
	for (u16 pY = 0; pY < clipHeight; ++pY) {              
		for (u16 pX = 0; pX < clipWidth; ++pX) {  
		
			grayLevel = *(src+ (offsetStartY+pY) * fontWidth+ offsetStartX+pX);			
			if (grayLevel > 0) {
				
				if (grayLevel == maxGrays) {
					// We just need to copy the few opaque pixels 
					bitmap->setPixel(x + offsetStartX + pX, y + offsetStartY + pY, sourceColour);
				} else {
				
					// Antialiasing for the many gray pixels
					// We get the existing colour 
					bitmapColour = bitmap->getPixel(x + offsetStartX + pX, y + offsetStartY + pY);
					
					u32 jump = transparencyJump[grayLevel];
					rgb = bitmapColour;
					
					s16 increment = r - (bitmapColour & 0x7C00);
					if (increment >= 0) {
						rgb += (((increment * grayLevel) >> 8) & 0x7C00);
						rgb += (jump & BIT(increment)) ? 0x0400 : 0;
					} else {
						rgb -= (((-increment * grayLevel) >> 8) & 0x7C00);
						rgb -= (jump & BIT(-increment)) ? 0x0400 : 0;                                
					}
					
					increment = g - (bitmapColour & 0x03E0);
					if (increment >= 0) {
						rgb += (((increment * grayLevel) >> 8) & 0x03E0); 
						rgb += (jump & BIT(increment)) ? 0x0020 : 0;
					} else {
						rgb -= (((-increment * grayLevel)>>8) & 0x03E0);
						rgb -= (jump & BIT(-increment)) ? 0x0020 : 0;                                
					}
					
					increment = b - (bitmapColour & 0x001F);
					if (increment >= 0) {
						rgb += (((increment * grayLevel) >> 8) & 0x001F);
						rgb += (jump & BIT(increment)) ? 0x0001 : 0;
					} else {
						rgb -= (((-increment * grayLevel) >> 8) & 0x001F);
						rgb -= (jump & BIT(-increment)) ? 0x0001 : 0;                                
					}               
					
					bitmap->setPixel(x + offsetStartX + pX, y + offsetStartY + pY, rgb);
				}                            
			}
		}
	}
		
	return output;
}
