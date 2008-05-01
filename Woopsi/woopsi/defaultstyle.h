#ifndef _DEFAULT_STYLE_H_
#define _DEFAULT_STYLE_H_

#include "woopsifuncs.h"

using namespace std;

class DefaultStyle {
public:
	static u16 backColour;						/**< Colour used as background */
	static u16 shineColour;						/**< Colour used as light bevel edge */
	static u16 highlightColour;					/**< Colour used as highlighted elements */
	static u16 shadowColour;					/**< Colour used as dark bevel edge */
	static u16 fillColour;						/**< Colour used as foreground fill */
	static u16 darkColour;						/**< Colour used as scrollbar gutters etc */
};

#endif
