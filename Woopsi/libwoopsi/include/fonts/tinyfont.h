#ifndef _TINYFONT_H_
#define _TINYFONT_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * TinyFont font.
	 */
	class TinyFont : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		TinyFont(u8 fixedWidth = 0);
	};
}

#endif
