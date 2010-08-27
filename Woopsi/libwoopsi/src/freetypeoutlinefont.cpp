#include "freetypeoutlinefont.h"
//#include "freetypefacemanager.h"
#include "stringiterator.h"

using namespace WoopsiUI;


FreeTypeOutlineFont::FreeTypeOutlineFont(FreeTypeCache* fontCache,const FaceId* faceId, const u8 width, const u8 height, const u16 colour) : FontBase(height, 0) { // Don't need transparency

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
	loadGlyph(' '); 		// Load the ' ' blank char...no bitmap, no drawing, but
	loadBox(); // metrics, fastest loading
}



u16 FreeTypeOutlineFont::getStringWidth(const WoopsiString& text) const {     	
	if ((!_faceId->isValid())|| !text.getLength()) return 0;
	
	u32 width = 0; 
	StringIterator* iterator = text.newStringIterator();
	
	do { 
		u32 codepoint = iterator->getCodePoint();
		
		if ((codepoint != 10) && (codepoint != 13)) {       
		      width += getCharWidth(codepoint);
		}
	}  while (iterator->moveToNext());
	
	delete iterator;
	return width;
}

u16 FreeTypeOutlineFont::getStringWidth(const WoopsiString& text, s32 startIndex, s32 length) const { 
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
		      width += getCharWidth(codepoint);
		}
		length--;
	} while ((iterator->moveToNext()) && (length > 0));
	
	delete iterator;
	return width;
};

typedef struct Data_ {
    MutableBitmapBase* bitmap;
    s16 x;
    s16 y;
    u16 color;
    u16 r;
    u16 g;
    u16 b;
} Data;

void renderDirectly(int y, int count, const FT_Span*  spans, void* user){
    
    Data* data = (Data*) user;
    u16 color = data->color;
    if (color < 0x8000) return;
    
    u16 r = data->r;
    u16 g = data->g;
    u16 b = data->b;

    MutableBitmapBase* bitmap  = data->bitmap;
    for (int a=0;a<count;a++){
        FT_Span span = spans[a];
        u8 grayLevel = span.coverage ;
        if (grayLevel  >0) {
            if (grayLevel  == 255) {
                for (int x = span.x;x < span.x+span.len; x++) bitmap->setPixel(x+data->x,data->y-y, color);
            } else {
                for (int x = span.x;x < span.x+span.len; x++){                
                    u16 bitmapColour = bitmap->getPixel(x+data->x,data->y-y);
                    extern u8 transparencyJump[256];
                    u32 jump = transparencyJump[grayLevel];
                    u16 rgb = bitmapColour;
                    
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
                    
                    bitmap->setPixel(x+data->x,data->y-y, rgb);
                }                            
            }  
       }
    }      
};

s16 FreeTypeOutlineFont::drawChar(MutableBitmapBase* bitmap, u32 letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
    if (!_faceId->isValid()) return x;
	switch (letter) {
		case 10:  // Line feed
		case 13:  // Carriage return
			return x;
		default:
			break;
	}

	//	s16 output = x + _glyph->advance.x;
		    s16 output =  x + getCharWidth(letter); // updates the Glyph in _glyph

		if (_glyph->format == FT_GLYPH_FORMAT_OUTLINE) {
        
        // access outline content by typecasting                        
        FT_OutlineGlyph glyph_outline = (FT_OutlineGlyph) _glyph;
        
        FT_Raster_Params params;
        
        FT_Bitmap myBitmap;
        myBitmap.rows = bitmap->getHeight();
        myBitmap.width = bitmap->getWidth();
        myBitmap.pitch = bitmap->getWidth()*2;
        myBitmap.buffer = (unsigned char*) bitmap->getData();
        myBitmap.num_grays = 255;
        myBitmap.pixel_mode = FT_PIXEL_MODE_GRAY;
        myBitmap.palette_mode = 0;
        myBitmap.palette = NULL;
        
        
                Data data;
        data.bitmap = bitmap;
        data.x = x;
        data.y = y;
 	// This is the colour of the font
	data.color = getColour();
	data.r = data.color & 0x7C00;
	data.g = data.color & 0x03E0;
	data.b = data.color & 0x001F;
	


        params.target = (const FT_Bitmap*) &myBitmap;
        params.source = glyph_outline;
        params.flags = FT_RASTER_FLAG_AA|FT_RASTER_FLAG_DIRECT ;
        params.gray_spans= renderDirectly;
        params.black_spans= NULL;
        params.bit_test= NULL;
        params.bit_set= NULL;
        params.user	 = (void*) &data;//bitmap;
        params.clip_box.xMin = clipX1;
        params.clip_box.xMax = clipX2;
        params.clip_box.yMin = clipY1;
        params.clip_box.yMax = clipY2;    
        _fontCache->renderGlyph(&glyph_outline->outline,&params);
        
        

	}
	return output;
};
