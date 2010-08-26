#ifndef _COURIER12B_H_
#define _COURIER12B_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Courier12b font.
	 */
	class Courier12b : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Courier12b(u8 fixedWidth = 0);
	};
}

#endif
