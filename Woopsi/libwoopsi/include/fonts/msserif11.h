#ifndef _MSSERIF11_H_
#define _MSSERIF11_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * MSSerif11 font.
	 */
	class MSSerif11 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		MSSerif11(u8 fixedWidth = 0);
	};
}

#endif
