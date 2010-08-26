#ifndef _COURIERNEW10_H_
#define _COURIERNEW10_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * CourierNew10 font.
	 */
	class CourierNew10 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		CourierNew10(u8 fixedWidth = 0);
	};
}

#endif
