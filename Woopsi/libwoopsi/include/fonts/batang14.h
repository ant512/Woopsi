#ifndef _BATANG14_H_
#define _BATANG14_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Batang14 font.
	 */
	class Batang14 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Batang14(u8 fixedWidth = 0);
	};
}

#endif
