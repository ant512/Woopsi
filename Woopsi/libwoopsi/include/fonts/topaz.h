#ifndef _TOPAZ_H_
#define _TOPAZ_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Topaz font.
	 */
	class Topaz : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Topaz(u8 fixedWidth = 0);
	};
}

#endif
