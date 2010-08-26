#ifndef _BATANG15_H_
#define _BATANG15_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Batang15 font.
	 */
	class Batang15 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Batang15(u8 fixedWidth = 0);
	};
}

#endif
