#ifndef _KARTIKA9_H_
#define _KARTIKA9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Kartika9 font.
	 */
	class Kartika9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Kartika9(u8 fixedWidth = 0);
	};
}

#endif
