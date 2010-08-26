#ifndef _COURIER12_H_
#define _COURIER12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Courier12 font.
	 */
	class Courier12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Courier12(u8 fixedWidth = 0);
	};
}

#endif
