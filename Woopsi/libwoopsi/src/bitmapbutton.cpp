#include "bitmapbutton.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "bitmapbase.h"

using namespace WoopsiUI;

BitmapButton::BitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, const BitmapBase* bitmapNormal, const BitmapBase* bitmapClicked) : Gadget(x, y, width, height, NULL) {
	_bitmapX = bitmapX;
	_bitmapY = bitmapY;
	_bitmapClicked = bitmapClicked;
	_bitmapNormal = bitmapNormal;
}

void BitmapButton::drawContents(GraphicsPort* port) {

	Rect rect;
	getClientRect(rect);

	const BitmapBase* bitmap = isClicked() ? _bitmapClicked : _bitmapNormal;

	if (isEnabled()) {
		port->drawBitmap(0, 0, rect.width, rect.height, bitmap, _bitmapX, _bitmapY);
	} else {
		port->drawBitmapGreyScale(0, 0, rect.width, rect.height, bitmap, _bitmapX, _bitmapY);
	}
}

void BitmapButton::drawBorder(GraphicsPort* port) {
	drawOutline(port);
}

void BitmapButton::drawOutline(GraphicsPort* port) {

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

void BitmapButton::onClick(s16 x, s16 y) {
	markRectsDamaged();
}

void BitmapButton::onRelease(s16 x, s16 y) {
	_gadgetEventHandlers->raiseReleaseEvent(x, y);
	markRectsDamaged();
}

void BitmapButton::onReleaseOutside(s16 x, s16 y) {
	markRectsDamaged();
}

// Get the preferred dimensions of the gadget
void BitmapButton::getPreferredDimensions(Rect& rect) const {
	rect.x = _rect.getX();
	rect.y = _rect.getY();
	rect.width = 0;
	rect.height = 0;

	if (!_flags.borderless) {
		rect.width = _borderSize.left + _borderSize.right;
		rect.height = _borderSize.top + _borderSize.bottom;
	}

	rect.width += _bitmapNormal->getWidth();
	rect.height += _bitmapNormal->getHeight();
}
