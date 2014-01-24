#include "pipebuttonblock.h"
#include "woopsifuncs.h"

PipeButtonBlock::PipeButtonBlock(s16 x, s16 y, u16 width, u16 height) : PipeButtonBase(x, y, width, height) {
	_flags.draggable = false;
}

void PipeButtonBlock::reveal() {
	PipeButtonBase::reveal();
	disable();
}

void PipeButtonBlock::drawContents(GraphicsPort* port) {

	// Do not draw if contents not revealed
	if (!isRevealed()) {
		drawUnrevealedContents(port);
		return;
	}

	Rect rect;
	getClientRect(rect);
	
	u16 colour;
	
	if (isEnabled()) {
		colour = getShadowColour();
	} else {
		colour = getDarkColour();
	}

	port->drawFilledRect(2, 2, rect.width - 4, rect.height - 4, colour);
}
