#ifndef _MSSANS9B_H_
#define _MSSANS9B_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * MSSans9b font.
	 */
	class MSSans9b : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		MSSans9b(u8 fixedWidth = 0);
	};
}

#endif
