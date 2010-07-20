#ifndef _TINYFONT_H_
#define _TINYFONT_H_

#include "packedfont1.h"

namespace WoopsiUI {
	class TinyFont : public PackedFont1 {
	public:
		TinyFont(u8 fixedWidth = 0);
	};
}

#endif
