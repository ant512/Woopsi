#ifndef _KARTIKA10_H_
#define _KARTIKA10_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Kartika10 font.
	 */
	class Kartika10 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Kartika10(u8 fixedWidth = 0);
	};
}

#endif
