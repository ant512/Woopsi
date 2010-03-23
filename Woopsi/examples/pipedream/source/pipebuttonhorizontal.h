#ifndef _PIPE_BUTTON_HORIZONTAL_H_
#define _PIPE_BUTTON_HORIZONTAL_H_

#include "pipebuttonstandard.h"
#include "graphicsport.h"
#include "constants.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

class PipeButtonHorizontal : public PipeButtonStandard {
public:
	PipeButtonHorizontal(s16 x, s16 y, u16 width, u16 height) : PipeButtonStandard(x, y, width, height, false, true, false, true) { };
	
protected:
	virtual ~PipeButtonHorizontal() { };

	void drawContents(GraphicsPort* port) {

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
		if (!hasAvailableLeftConnector()) {
			// Flow is moving right
			port->drawFilledRect(0, ((rect.height - FLOW_SIZE) / 2) + 1, getFlowLevel(), FLOW_SIZE, woopsiRGB(0, 0, 20));
		} else {
			// Flow is moving left
			port->drawFilledRect(_width - getFlowLevel() - 1, ((rect.height - FLOW_SIZE) / 2) + 1, getFlowLevel(), FLOW_SIZE, woopsiRGB(0, 0, 20));
		}

		port->drawHorizLine(0, (rect.height - FLOW_SIZE) / 2, rect.width, colour);
		port->drawHorizLine(0, (rect.height + FLOW_SIZE) / 2, rect.width, colour);
	}
};
	
#endif