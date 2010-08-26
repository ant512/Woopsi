#ifndef _MODERN9B_H_
#define _MODERN9B_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Modern9b font.
	 */
	class Modern9b : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Modern9b(u8 fixedWidth = 0);
	};
}

#endif
