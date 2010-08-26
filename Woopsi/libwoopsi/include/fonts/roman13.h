#ifndef _ROMAN13_H_
#define _ROMAN13_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Roman13 font.
	 */
	class Roman13 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Roman13(u8 fixedWidth = 0);
	};
}

#endif
