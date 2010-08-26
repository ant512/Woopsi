#ifndef _MSGOTHIC8_H_
#define _MSGOTHIC8_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * MSGothic8 font.
	 */
	class MSGothic8 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		MSGothic8(u8 fixedWidth = 0);
	};
}

#endif
