#include "progressbar.h"
#include "graphicsport.h"
#include <stdio.h>

using namespace WoopsiUI;

ProgressBar::ProgressBar(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, 0) {
	_minimumValue = 0;
	_maximumValue = 0;
	_value = 0;
	_showPercentageText = true;

	_flags.borderless = false;
}

void ProgressBar::setValue(const s16 value) {

	_value = value;

	// Limit to max/min values
	if (_value > _maximumValue) _value = _maximumValue;
	if (_value < _minimumValue) _value = _minimumValue;

	redraw();
}

void ProgressBar::drawContents(GraphicsPort* port) {

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
	port->drawFilledRect(0, 0, pixelValue, rect.height, getHighlightColour());
	
	// Draw unfilled background
	if (pixelValue < rect.width) {
		port->drawFilledRect(pixelValue, 0, rect.width - pixelValue, rect.height, getBackColour());
	}

	// Draw completion percentage text
	if (_showPercentageText) {
		char text[4];
		sprintf(text, "%d%%", (100 * _value) / (_maximumValue - _minimumValue));

		s16 textX = (rect.width - getFont()->getStringWidth(text)) >> 1;
		s16 textY = (rect.height - getFont()->getHeight()) >> 1;

		if (isEnabled()) {
			port->drawText(textX, textY, getFont(), text);
		} else {
			port->drawText(textX, textY, getFont(), text, 0, strlen(text), getDarkColour());
		}
	}
}

void ProgressBar::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShadowColour(), getShineColour());
}
