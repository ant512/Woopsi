#ifndef _MSGOTHIC9_H_
#define _MSGOTHIC9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * MSGothic9 font.
	 */
	class MSGothic9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		MSGothic9(u8 fixedWidth = 0);
	};
}

#endif
