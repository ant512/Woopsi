#ifndef _MSSANS9_H_
#define _MSSANS9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * MSSans9 font.
	 */
	class MSSans9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		MSSans9(u8 fixedWidth = 0);
	};
}

#endif
