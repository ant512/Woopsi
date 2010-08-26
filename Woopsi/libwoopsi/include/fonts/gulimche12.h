#ifndef _GULIMCHE12_H_
#define _GULIMCHE12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Gulimche12 font.
	 */
	class Gulimche12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Gulimche12(u8 fixedWidth = 0);
	};
}

#endif
