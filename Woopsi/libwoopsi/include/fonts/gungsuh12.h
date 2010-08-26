#ifndef _GUNGSUH12_H_
#define _GUNGSUH12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Gungsuh12 font.
	 */
	class Gungsuh12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Gungsuh12(u8 fixedWidth = 0);
	};
}

#endif
