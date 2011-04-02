#ifndef _GADGET_STYLE_H_
#define _GADGET_STYLE_H_

#include <nds.h>

namespace WoopsiUI {

	class FontBase;

	/**
	 * Class containing all colours that a gadget depends on.
	 */
	class GadgetColours {
	public:
		u16 back;				/**< Colour used as background. */
		u16 shine;				/**< Colour used as light bevel edge. */
		u16 highlight;			/**< Colour used as highlighted elements. */
		u16 shadow;				/**< Colour used as dark bevel edge. */
		u16 fill;				/**< Colour used as foreground fill. */
		u16 dark;				/**< Colour used as scrollbar gutters etc. */
		u16 text;				/**< Colour used for text. */
	};

	/**
	 * Class providing the default colours and fonts for all gadgets.  This
	 * information is copied into the gadgets when they are constructed, so you
	 * will need to change these values *before* creating any gadgets if you
	 * want those gadgets to use the altered colours.
	 * If the font objects are set, Woopsi will use them in preference to the
	 * default system fonts.
	 */
	class GadgetStyle {
	public:
		GadgetColours colours;				/**< All gadget colours. */
		FontBase* font;						/**< Default font. */
		FontBase* glyphFont;				/**< Default glyph font. */
	};
}

#endif
