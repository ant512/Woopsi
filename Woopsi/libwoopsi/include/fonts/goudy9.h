#ifndef _GOUDY9_H_
#define _GOUDY9_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Goudy9 font.
	 */
	class Goudy9 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Goudy9(u8 fixedWidth = 0);
	};
}

#endif
