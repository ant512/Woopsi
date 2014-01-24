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
	markRectsDamaged();
	disable();
}

void PipeButtonStandard::drawContents(GraphicsPort* port) {

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

	if (hasTopConnector()) {
		
		s16 x1 = (rect.width - FLOW_SIZE) / 2;
		s16 y1 = 0;
		s16 x2 = x1;
		s16 y2 = y1 + (rect.height / 2) - 1;
		
		port->drawLine(x1, y1, x2, y2, colour);
		
		// Draw flow
		port->drawFilledRect(x1, y1, FLOW_SIZE, getFlowLevel(), woopsiRGB(0, 0, 20));
		
		x1 = (rect.width + FLOW_SIZE) / 2;
		x2 = x1;
		
		port->drawLine(x1, y1, x2, y2, colour);
	}
	
	if (hasBottomConnector()) {
		
		s16 x1 = (rect.width - FLOW_SIZE) / 2;
		s16 y1 = rect.height / 2;
		s16 x2 = x1;
		s16 y2 = y1 + (rect.height / 2) - 1;
		
		port->drawLine(x1, y1, x2, y2, colour);
		
		// Draw flow
		port->drawFilledRect(x1, y1, FLOW_SIZE, getFlowLevel(), woopsiRGB(0, 0, 20));
		
		x1 = (rect.width + FLOW_SIZE) / 2;
		x2 = x1;
		
		port->drawLine(x1, y1, x2, y2, colour);
	}
	
	if (hasLeftConnector()) {
	
		s16 x1 = 0;
		s16 y1 = rect.height / 2;
		s16 x2 = x1 + (rect.width / 2) - 1;
		s16 y2 = y1;
		
		port->drawLine(x1, y1, x2, y2, colour);
	}
	
	if (hasRightConnector()) {
	
		s16 x1 = rect.width / 2;
		s16 y1 = rect.height / 2;
		s16 x2 = x1 + (rect.width / 2) - 1;
		s16 y2 = y1;
		
		port->drawLine(x1, y1, x2, y2, colour);
	}
}
