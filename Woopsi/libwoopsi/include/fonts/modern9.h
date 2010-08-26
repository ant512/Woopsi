#ifndef _MODERN9_H_
#define _MODERN9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Modern9 font.
	 */
	class Modern9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Modern9(u8 fixedWidth = 0);
	};
}

#endif
