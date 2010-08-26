#ifndef _OCRFONT8_H_
#define _OCRFONT8_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * OCRFont8 font.
	 */
	class OCRFont8 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		OCRFont8(u8 fixedWidth = 0);
	};
}

#endif
