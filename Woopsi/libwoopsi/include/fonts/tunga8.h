#ifndef _TUNGA8_H_
#define _TUNGA8_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Tunga8 font.
	 */
	class Tunga8 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Tunga8(u8 fixedWidth = 0);
	};
}

#endif
