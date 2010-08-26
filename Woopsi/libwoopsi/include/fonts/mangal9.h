#ifndef _MANGAL9_H_
#define _MANGAL9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Mangal9 font.
	 */
	class Mangal9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Mangal9(u8 fixedWidth = 0);
	};
}

#endif
