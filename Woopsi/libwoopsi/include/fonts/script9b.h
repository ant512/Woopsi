#ifndef _SCRIPT9B_H_
#define _SCRIPT9B_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Script9b font.
	 */
	class Script9b : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Script9b(u8 fixedWidth = 0);
	};
}

#endif
