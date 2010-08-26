#ifndef _GUNGSUHCHE15_H_
#define _GUNGSUHCHE15_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Gungsuhche15 font.
	 */
	class Gungsuhche15 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Gungsuhche15(u8 fixedWidth = 0);
	};
}

#endif
