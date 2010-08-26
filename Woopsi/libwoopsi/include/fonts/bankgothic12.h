#ifndef _BANKGOTHIC12_H_
#define _BANKGOTHIC12_H_

#include "packedfont1.h"

namespace WoopsiUI {

	/**
	 * BankGothic12 font.
	 */
	class BankGothic12 : public PackedFont1 {
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or 1 for fixed width.
		 */
		BankGothic12(u8 fixedWidth = 0);
	};
}

#endif
