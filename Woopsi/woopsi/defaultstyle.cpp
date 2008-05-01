#include "defaultstyle.h"
#include "defines.h"

u16 DefaultStyle::backColour = woopsiRGB(20, 20, 20);
u16 DefaultStyle::shineColour = woopsiRGB(31, 31, 31);
u16 DefaultStyle::highlightColour = woopsiRGB(12, 17, 23);
u16 DefaultStyle::shadowColour = woopsiRGB(0, 0, 0);
u16 DefaultStyle::fillColour = woopsiRGB(24, 24, 24);
u16 DefaultStyle::darkColour = woopsiRGB(15, 15, 15);

FontBase* DefaultStyle::font = NULL;
