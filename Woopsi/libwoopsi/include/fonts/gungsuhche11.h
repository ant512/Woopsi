#ifndef _GUNGSUHCHE11_H_
#define _GUNGSUHCHE11_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Gungsuhche11 font.
	 */
	class Gungsuhche11 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Gungsuhche11(u8 fixedWidth = 0);
	};
}

#endif
