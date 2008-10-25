#include "progressbar.h"
#include "graphicsport.h"

using namespace WoopsiUI;

ProgressBar::ProgressBar(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, 0) {
	_outline = OUTLINE_IN;
	_minimumValue = 0;
	_maximumValue = 0;
	_value = 0;

	_flags.borderless = false;
}

void ProgressBar::setValue(const s16 value) {

	_value = value;

	// Limit to max/min values
	if (_value > _maximumValue) _value = _maximumValue;
	if (_value < _minimumValue) _value = _minimumValue;

	draw();
}

void ProgressBar::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Calculate the size of the progress bar's filled and unfilled rects
	// Convert the value to co-ordinates using fixed-point fractional values
	// for accuracy
	Rect rect;
	getClientRect(rect);

	// Calculate ratio of pixels to value range (max fractional value of 255)
	u32 ratio = (rect.width << 8) / (u32)(_maximumValue - _minimumValue);
	
	// Convert value using ratio
	s16 pixelValue = (_value * ratio);

	// Round up
	pixelValue += pixelValue & 128;

	// Bitshift down
	pixelValue >>= 8;

	// Draw filled region
	port->drawFilledRect(rect.x, rect.y, pixelValue, rect.height, _highlightColour);
	
	// Draw unfilled background
	if (pixelValue < rect.width) {
		port->drawFilledRect(rect.x + pixelValue, rect.y, rect.width - pixelValue, rect.height, _backColour);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}
