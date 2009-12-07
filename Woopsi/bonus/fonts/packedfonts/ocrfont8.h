
//
// ocrfont8.h
//
// Do not edit - created by 'bmp2font' from bitmaps/ocrfont8.bmp
//

#ifndef _OCR_FONT_8_H_
#define _OCR_FONT_8_H_

#include "packedfont16.h"

namespace WoopsiUI {
	class OCRFont8: public PackedFont16 {
	public:
		OCRFont8(u8 fixedWidth=0);
	};
}

#endif
