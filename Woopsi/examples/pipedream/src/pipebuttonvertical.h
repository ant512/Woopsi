#ifndef _PIPE_BUTTON_VERTICAL_H_
#define _PIPE_BUTTON_VERTICAL_H_

#include "pipebuttonstandard.h"
#include "graphicsport.h"
#include "constants.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

class PipeButtonVertical : public PipeButtonStandard {
public:
	PipeButtonVertical(s16 x, s16 y, u16 width, u16 height) : PipeButtonStandard(x, y, width, height, true, false, true, false) { };
	
protected:
	virtual ~PipeButtonVertical() { };
	
	void drawContents(GraphicsPort* port) {

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
		
		// Draw flow
		if (!hasAvailableTopConnector()) {
			// Flow is moving downwards
			port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, 0, FLOW_SIZE, getFlowLevel(), woopsiRGB(0, 0, 20));
		} else {
			// Flow is moving upwards
			port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, getHeight() - getFlowLevel() - 1, FLOW_SIZE, getFlowLevel(), woopsiRGB(0, 0, 20));
		}
		
		s16 x1 = (rect.width - FLOW_SIZE) / 2;
		s16 y1 = 0;
		s16 x2 = x1;
		s16 y2 = x1 + rect.height - 1;

		port->drawLine(x1, y1, x2, y2, colour);
		
		x1 = (rect.width + FLOW_SIZE) / 2;
		x2 = x1;
		
		port->drawLine(x1, y1, x2, y2, colour);
	}
};
	
#endif