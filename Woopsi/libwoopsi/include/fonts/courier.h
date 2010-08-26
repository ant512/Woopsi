#ifndef _COURIER_H_
#define _COURIER_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Courier font.
	 */
	class Courier : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Courier(u8 fixedWidth = 0);
	};
}

#endif
