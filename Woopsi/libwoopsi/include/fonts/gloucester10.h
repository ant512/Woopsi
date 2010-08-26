#ifndef _GLOUCESTER10_H_
#define _GLOUCESTER10_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * Gloucester10 font.
	 */
	class Gloucester10 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		Gloucester10(u8 fixedWidth = 0);
	};
}

#endif
