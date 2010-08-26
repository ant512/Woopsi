#ifndef _GILLSANS11_H_
#define _GILLSANS11_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * GillSans11 font.
	 */
	class GillSans11 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		GillSans11(u8 fixedWidth = 0);
	};
}

#endif
