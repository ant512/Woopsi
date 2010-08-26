#ifndef _PROFONT9_H_
#define _PROFONT9_H_

#include "packedfont16.h"

namespace WoopsiUI {

	/**
	 * ProFont9 font.
	 */
	class ProFont9 : public PackedFont16 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		ProFont9(u8 fixedWidth = 0);
	};
}

#endif
