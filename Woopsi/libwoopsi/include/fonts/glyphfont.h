#ifndef _GLYPHFONT_H_
#define _GLYPHFONT_H_

#include "packedfont16.h"

namespace WoopsiUI {

	/**
	 * GlyphFont font.
	 */
	class GlyphFont : public PackedFont16 {
	public:
		/**
		 * Constructor.
		 */
		GlyphFont();
	};
}

#endif
