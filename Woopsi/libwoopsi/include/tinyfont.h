#ifndef _TINYFONT_H_
#define _TINYFONT_H_

#include "monofont.h"

namespace WoopsiUI {
	class TinyFont : public MonoFont {
	public:
		TinyFont(u16 drawColour = 32768);
	};
}

#endif
