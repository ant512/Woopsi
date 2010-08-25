#ifndef _PROFONT9_H_
#define _PROFONT9_H_

#include "packedfont16.h"

namespace WoopsiUI {
	class ProFont9 : public PackedFont16 {
	public:
		ProFont9(u8 fixedWidth = 0);
	};
}

#endif
