#include "calendardaybutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

CalendarDayButton::CalendarDayButton(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style) : Button(x, y, width, height, text, style) {
	_borderSize.top = 1;
	_borderSize.right = 1;
	_borderSize.bottom = 1;
	_borderSize.right = 1;

	_isStuckDown = false;
}

void CalendarDayButton::drawOutline(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;
	
	// Work out which colours to use
	u16 col1;
	u16 col2;
	
	if (isClicked() || _isStuckDown) {
		// Bevelled into the screen
		col1 = getShadowColour();
		col2 = getShineColour();
	} else {
		// Bevelled out of the screen
		col1 = getShineColour();
		col2 = getShadowColour();
	}
	
	port->drawBevelledRect(0, 0, _width, _height, col1, col2);
}
