#ifndef _POORRICHARD9_H_
#define _POORRICHARD9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * PoorRichard9 font.
	 */
	class PoorRichard9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		PoorRichard9(u8 fixedWidth = 0);
	};
}

#endif
