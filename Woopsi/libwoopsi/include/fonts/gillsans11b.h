#ifndef _GILLSANS11B_H_
#define _GILLSANS11B_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * GillSans11b font.
	 */
	class GillSans11b : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		GillSans11b(u8 fixedWidth = 0);
	};
}

#endif
