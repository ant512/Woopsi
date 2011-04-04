#include "button.h"
#include "graphicsport.h"
#include "woopsi.h"

using namespace WoopsiUI;

Button::Button(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style) : Label(x, y, width, height, text, style) {
	setBorderless(false);
}

void Button::drawContents(GraphicsPort* port) {

	u16 textColour;

	if (!isEnabled()) {
		textColour = getDarkColour();
	} else if (!isClicked()) {
		textColour = getTextColour();
	} else {
		textColour = getShineColour();
	}

	port->drawText(_textX, _textY, getFont(), _text, 0, _text.getLength(), textColour);
}

void Button::drawBorder(GraphicsPort* port) {

	// Determine the background colour
	u16 colour;
	if (isClicked()) {
		colour = getDarkColour();
	} else {
		colour = getBackColour();
	}

	port->drawFilledRect(0, 0, getWidth(), getHeight(), colour);

	drawOutline(port);
}

void Button::drawOutline(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;
	
	// Work out which colours to use
	u16 col1;
	u16 col2;
	
	if (isClicked()) {
		// Bevelled into the screen
		col1 = getShadowColour();
		col2 = getShineColour();
	} else {
		// Bevelled out of the screen
		col1 = getShineColour();
		col2 = getShadowColour();
	}
	
	port->drawBevelledRect(0, 0, getWidth(), getHeight(), col1, col2);
}

void Button::onClick(s16 x, s16 y) {
	markRectsDamaged();
}

void Button::onRelease(s16 x, s16 y) {
	_gadgetEventHandlers->raiseActionEvent();
	markRectsDamaged();
}

void Button::onReleaseOutside(s16 x, s16 y) {
	markRectsDamaged();
}
