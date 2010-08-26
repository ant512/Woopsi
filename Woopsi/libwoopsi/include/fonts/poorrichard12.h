#ifndef _POORRICHARD12_H_
#define _POORRICHARD12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * PoorRichard12 font.
	 */
	class PoorRichard12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		PoorRichard12(u8 fixedWidth = 0);
	};
}

#endif
