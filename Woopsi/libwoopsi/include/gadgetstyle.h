#ifndef _GADGET_STYLE_H_
#define _GADGET_STYLE_H_

#include <nds.h>

namespace WoopsiUI {

	class FontBase;

	/**
	 * Class providing the default colours and fonts for all gadgets.  This information is copied
	 * into the gadgets when they are constructed, so you will need to change these values *before*
	 * creating any gadgets if you want those gadgets to use the altered colours.
	 * If the font object is set, Woopsi will use it in preference to the default system font.
	 */
	class GadgetStyle {
	public:
		u16 backColour;						/**< Colour used as background */
		u16 shineColour;					/**< Colour used as light bevel edge */
		u16 highlightColour;				/**< Colour used as highlighted elements */
		u16 shadowColour;					/**< Colour used as dark bevel edge */
		u16 fillColour;						/**< Colour used as foreground fill */
		u16 darkColour;						/**< Colour used as scrollbar gutters etc */
		FontBase* font;						/**< Default font */
	};
}

#endif
