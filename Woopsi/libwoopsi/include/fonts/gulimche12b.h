#ifndef _GULIMCHE12B_H_
#define _GULIMCHE12B_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Gulimche12b font.
	 */
	class Gulimche12b : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Gulimche12b(u8 fixedWidth = 0);
	};
}

#endif
