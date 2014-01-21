/* Contributed by Steven
 * 20071213
 * - Minor modifications and SDL code by Ant
 */

#include <nds.h>
#include <string.h>
#include "woopsifuncs.h"
#include "graphics.h"

#ifdef USING_SDL

#include "defines.h"

WoopsiUI::GadgetStyle* defaultGadgetStyle;

int fatInitDefault() { return 1; }

#else

// Using libnds

#endif

void woopsiInitDefaultGadgetStyle() {
	defaultGadgetStyle = new WoopsiUI::GadgetStyle();

	defaultGadgetStyle->colours.back = woopsiRGB(20, 20, 20);
	defaultGadgetStyle->colours.shine = woopsiRGB(31, 31, 31);
	defaultGadgetStyle->colours.highlight = woopsiRGB(12, 17, 23);
	defaultGadgetStyle->colours.shadow = woopsiRGB(0, 0, 0);
	defaultGadgetStyle->colours.fill = woopsiRGB(24, 24, 24);
	defaultGadgetStyle->colours.dark = woopsiRGB(15, 15, 15);
	defaultGadgetStyle->colours.text = 0;
	defaultGadgetStyle->font = new WoopsiUI::NewTopaz();
	defaultGadgetStyle->glyphFont = new WoopsiUI::GlyphFont();
}

void woopsiFreeDefaultGadgetStyle() {
	delete defaultGadgetStyle->font;
	delete defaultGadgetStyle->glyphFont;
	delete defaultGadgetStyle;
}
