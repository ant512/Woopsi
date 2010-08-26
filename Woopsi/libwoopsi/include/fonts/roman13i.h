#ifndef _ROMAN13I_H_
#define _ROMAN13I_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Roman13i font.
	 */
	class Roman13i : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Roman13i(u8 fixedWidth = 0);
	};
}

#endif
