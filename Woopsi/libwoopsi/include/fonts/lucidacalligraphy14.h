#ifndef _LUCIDACALLIGRAPHY14_H_
#define _LUCIDACALLIGRAPHY14_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * LucidaCalligraphy14 font.
	 */
	class LucidaCalligraphy14 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		LucidaCalligraphy14(u8 fixedWidth = 0);
	};
}

#endif
