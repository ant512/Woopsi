#ifndef _FIXEDSYS12_H_
#define _FIXEDSYS12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * FixedSys12 font.
	 */
	class FixedSys12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		FixedSys12(u8 fixedWidth = 0);
	};
}

#endif
