#ifndef _GARAMOND10_H_
#define _GARAMOND10_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Garamond10 font.
	 */
	class Garamond10 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Garamond10(u8 fixedWidth = 0);
	};
}

#endif
