#ifndef _COURIERNEW12_H_
#define _COURIERNEW12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * CourierNew12 font.
	 */
	class CourierNew12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		CourierNew12(u8 fixedWidth = 0);
	};
}

#endif
