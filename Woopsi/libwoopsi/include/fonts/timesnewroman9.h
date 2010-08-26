#ifndef _TIMESNEWROMAN9_H_
#define _TIMESNEWROMAN9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * TimesNewRoman9 font.
	 */
	class TimesNewRoman9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		TimesNewRoman9(u8 fixedWidth = 0);
	};
}

#endif
