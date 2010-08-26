#ifndef _ROMAN9B_H_
#define _ROMAN9B_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Roman9b font.
	 */
	class Roman9b : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Roman9b(u8 fixedWidth = 0);
	};
}

#endif
