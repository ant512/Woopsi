#include "bitmapbutton.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "bitmapbase.h"

using namespace WoopsiUI;

BitmapButton::BitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, const BitmapBase* bitmapNormal, const BitmapBase* bitmapClicked) : Gadget(x, y, width, height, 0, NULL) {
	_outline = OUTLINE_CLICK_DEPENDENT;

	_bitmapX = bitmapX;
	_bitmapY = bitmapY;
	_bitmapClicked = bitmapClicked;
	_bitmapNormal = bitmapNormal;
}

void BitmapButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	s16 x = !_flags.borderless;
	s16 y = x;

	if (_flags.clicked) {
		port->drawBitmap(x, y, _width, _height, _bitmapClicked, _bitmapX, _bitmapY);
	} else {
		port->drawBitmap(x, y, _width, _height, _bitmapNormal, _bitmapX, _bitmapY);
	}

	// Grey out the button if it is disabled
	if (!isEnabled()) {
		port->greyScale(x, y, _width, _height);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool BitmapButton::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		if (isEnabled()) {
	 		redraw();
		}

		return true;
	}

	return false;
}

bool BitmapButton::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;
		_flags.dragging = false;

		if (woopsiApplication->getClickedGadget() == this) {
			woopsiApplication->setClickedGadget(NULL);
		}

		// Determine which release event to fire
		if (checkCollision(x, y)) {
			// Release occurred within gadget; raise release
			raiseReleaseEvent(x, y);

			// Also raise "action" event
			raiseActionEvent(x, y, 0, 0, KEY_CODE_NONE);
		} else {
			// Release occurred outside gadget; raise release
			raiseReleaseOutsideEvent(x, y);
		}

		redraw();

		return true;
	}

	return false;
}

// Get the preferred dimensions of the gadget
void BitmapButton::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = ((!_flags.borderless) << 1) + _bitmapNormal->getWidth();
	rect.height = ((!_flags.borderless) << 1) + _bitmapNormal->getHeight();
}
