#ifndef _LATHA9_H_
#define _LATHA9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Latha9 font.
	 */
	class Latha9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Latha9(u8 fixedWidth = 0);
	};
}

#endif
