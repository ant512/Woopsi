#ifndef _ARIAL9_H_
#define _ARIAL9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Arial9 font.
	 */
	class Arial9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Arial9(u8 fixedWidth = 0);
	};
}

#endif
