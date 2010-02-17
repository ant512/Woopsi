/* Contributed by Jeff Laing
 * 20071207
 * - Updated by Ant to follow Woopsi code style
 */

#include "gradient.h"
#include "graphicsport.h"

using namespace WoopsiUI;

// Constructor
Gradient::Gradient(s16 x, s16 y, u16 width, u16 height, u16 fromColour, u16 toColour)
  :	Gadget(x, y, width, height, GADGET_BORDERLESS),
	_fromColour(fromColour),
	_toColour(toColour) {
	
	_flags.decoration = 1;
	
	_rowColour = NULL;
	
	calculate();
}

// Calculate gradient values
void Gradient::calculate() {

	// Compute initial and final states and appropriate deltas
	u8 fromB = (_fromColour >> 10) & 0x1F;
	u8 fromG = (_fromColour >> 5) & 0x1F;
	u8 fromR = _fromColour & 0x1F;

	u8 toB = (_toColour >> 10) & 0x1F;
	u8 toG = (_toColour >> 5) & 0x1F;
	u8 toR = _toColour & 0x1F;

	// Work out how many rows we need to draw in each color before
	// incrementing that color component
	_numberR = _height / (abs(toR - fromR) + 1);
	_numberG = _height / (abs(toG - fromG) + 1);
	_numberB = _height / (abs(toB - fromB) + 1);

	_deltaR = toR < fromR ? -1 : 1;
	_deltaG = toG < fromG ? -(1 << 5) : 1 << 5;
	_deltaB = toB < fromB ? -(1 << 10) : 1 << 10;
	
	cacheRowColours();
}

// Precalculate the colour of each row in the gradient
void Gradient::cacheRowColours() {
	
	// Delete existing row cache if necessary
	if (_rowColour != NULL) { delete [] _rowColour; }
	
	// Allocate new cache
	_rowColour = new s16[_height];
	
	// Calculate the colours for each row
	s16 colourR, colourG, colourB;
		
	colourR = _numberR;
	colourG = _numberG;
	colourB = _numberB;
	
	// Set initial row colour
	_rowColour[0] = _fromColour;
	
	for (u16 y = 0; y < _height; y++) {
	
		// Set the colour of this row equal to the colour of the last row
		if (y > 0) { _rowColour[y] = _rowColour[y - 1]; }
	
		// Calculate next line colour
		if (colourR-- < 0) { colourR = _numberR; _rowColour[y] += _deltaR; }
		if (colourG-- < 0) { colourG = _numberG; _rowColour[y] += _deltaG; }
		if (colourB-- < 0) { colourB = _numberB; _rowColour[y] += _deltaB; }
	}
}

void Gradient::drawContents(GraphicsPort* port) {

	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	Rect rect;
	port->getClipRect(rect);
		
	u16 row = rect.y;

	// Loop through all lines within this clipping region
	u16 y = 0;		
	while (y < rect.height) {
	
		// Draw line (y value adjusted back to gadget space from screen space)
		port->drawHorizLine(rect.x, row, rect.width, _rowColour[row]);

		row++;
		y++;
	}
}

void Gradient::onResize(u16 width, u16 height) {
	calculate();
}
