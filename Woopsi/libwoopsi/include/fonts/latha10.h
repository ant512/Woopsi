#ifndef _LATHA10_H_
#define _LATHA10_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Latha10 font.
	 */
	class Latha10 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Latha10(u8 fixedWidth = 0);
	};
}

#endif
