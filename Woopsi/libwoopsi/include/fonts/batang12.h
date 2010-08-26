#ifndef _BATANG12_H_
#define _BATANG12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Batang12 font.
	 */
	class Batang12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Batang12(u8 fixedWidth = 0);
	};
}

#endif
