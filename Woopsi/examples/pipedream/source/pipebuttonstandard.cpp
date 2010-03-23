#include "pipebuttonstandard.h"
#include "woopsifuncs.h"

PipeButtonStandard::PipeButtonStandard(s16 x, s16 y, u16 width, u16 height, PipeButtonBase::Connectors connectors) : PipeButtonBase(x, y, width, height) {
	setConnectors(connectors);
}

PipeButtonStandard::PipeButtonStandard(s16 x, s16 y, u16 width, u16 height, bool top, bool right, bool bottom, bool left) : PipeButtonBase(x, y, width, height) {
	PipeButtonBase::Connectors connectors;
	connectors.top = top;
	connectors.right = right;
	connectors.bottom = bottom;
	connectors.left = left;
	setConnectors(connectors);
}

void PipeButtonStandard::increaseFlowLevel(u8 increase) {
	_flowLevel += increase;
	redraw();
	disable();
}

void PipeButtonStandard::drawContents(GraphicsPort* port) {

	// Do not draw if contents not revealed
	if (!isRevealed()) return;

	Rect rect;
	getClientRect(rect);
	
	u16 colour;
	
	if (isEnabled()) {
		colour = getShadowColour();
	} else {
		colour = getDarkColour();
	}
	
	// Draw flow
	port->drawFilledRect(0, 0, getFlowLevel(), _height, woopsiRGB(0, 0, 20));

	if (hasTopConnector()) {
		port->drawVertLine(rect.width / 2, 0, rect.height / 2, colour);
	}
	
	if (hasBottomConnector()) {
		port->drawVertLine(rect.width / 2, rect.height / 2, rect.height / 2, colour);
	}
	
	if (hasLeftConnector()) {
		port->drawHorizLine(0, rect.height / 2, rect.width / 2, colour);
	}
	
	if (hasRightConnector()) {
		port->drawHorizLine(rect.width / 2, rect.height / 2, rect.width / 2, colour);
	}
}
