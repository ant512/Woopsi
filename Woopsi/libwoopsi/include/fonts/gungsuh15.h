#ifndef _GUNGSUH15_H_
#define _GUNGSUH15_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Gungsuh15 font.
	 */
	class Gungsuh15 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Gungsuh15(u8 fixedWidth = 0);
	};
}

#endif
