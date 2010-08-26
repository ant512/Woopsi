#ifndef _DOTUM13_H_
#define _DOTUM13_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Dotum13 font.
	 */
	class Dotum13 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Dotum13(u8 fixedWidth = 0);
	};
}

#endif
