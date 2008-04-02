#ifndef _DECORATION_GLYPH_BUTTON_H_
#define _DECORATION_GLYPH_BUTTON_H_

#include <nds.h>
#include "button.h"

using namespace std;

/**
 * Class representing a button that will display a glyph and form part
 * of another gadget's border decorations.  Examples include the
 * screen flip/depth buttons.
 */
class DecorationGlyphButton : public Button {

public:
	DecorationGlyphButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, FontBase* font = NULL);
	
	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual bool focus();
	virtual bool release(s16 x, s16 y);

protected:
	char _normalGlyph;
	char _clickedGlyph;

	/**
	 * Destructor.
	 */
	virtual inline ~DecorationGlyphButton() { };
};

#endif
