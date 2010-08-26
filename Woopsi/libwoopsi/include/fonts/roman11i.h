#ifndef _ROMAN11I_H_
#define _ROMAN11I_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Roman11i font.
	 */
	class Roman11i : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Roman11i(u8 fixedWidth = 0);
	};
}

#endif
