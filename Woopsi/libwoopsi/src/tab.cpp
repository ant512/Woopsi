#include "tab.h"
#include "graphicsport.h"

using namespace WoopsiUI;

Tab::Tab(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style) : StickyButton(x, y, width, height, text, style) {
}

void Tab::activate() {
	setStuckDown(true);
	markRectsDamaged();
}

void Tab::deactivate() {
	setStuckDown(false);
	markRectsDamaged();
}

void Tab::drawContents(GraphicsPort* port) {
	
	u16 textColour;
	
	if (!isEnabled()) {
		textColour = getDarkColour();
	} else if (!isClicked() && !_isStuckDown) {
		textColour = getTextColour();
	} else {
		textColour = getShineColour();
	}
	
	port->drawText(_textX, _textY, getFont(), _text, 0, _text.getLength(), textColour);
}

void Tab::drawBorder(GraphicsPort* port) {
	
	// Determine the background colour
	u16 colour;
	if (isClicked() || _isStuckDown) {
		colour = getDarkColour();
	} else {
		colour = getBackColour();
	}
	
	port->drawPixel(0, 0, getBackColour());
	port->drawPixel(1, 0, getBackColour());
	port->drawPixel(0, 1, getBackColour());
	port->drawPixel(getWidth() - 2, 0, getBackColour());
	port->drawPixel(getWidth() - 1, 0, getBackColour());
	port->drawPixel(getWidth() - 1, 1, getBackColour());
	
	port->drawFilledRect(1, 1, getWidth() - 2, getHeight(), colour);
	
	drawOutline(port);
}

void Tab::drawOutline(GraphicsPort* port) {
	
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
	
	port->drawLine(0, 2, 0, getHeight() - 1, col1);
	port->drawPixel(1, 1, col1);
	port->drawLine(2, 0, getWidth() - 3, 0, col1);
	port->drawPixel(getWidth() - 2, 1, col2);
	port->drawLine(getWidth() - 1, 2, getWidth() - 1, getHeight() - 1, col2);
}

void Tab::onClick(s16 x, s16 y) {
	Button::onClick(x, y);
	activate();
}
