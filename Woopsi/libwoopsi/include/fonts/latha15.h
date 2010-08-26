#ifndef _LATHA15_H_
#define _LATHA15_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Latha15 font.
	 */
	class Latha15 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Latha15(u8 fixedWidth = 0);
	};
}

#endif
