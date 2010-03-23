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
		if (!hasAvailableTopConnector()) {
			// Flow is moving downwards
			port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, 0, FLOW_SIZE, getFlowLevel(), woopsiRGB(0, 0, 20));
		} else {
			// Flow is moving upwards
			port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, _height - getFlowLevel() - 1, FLOW_SIZE, getFlowLevel(), woopsiRGB(0, 0, 20));
		}

		port->drawVertLine((rect.width - FLOW_SIZE) / 2, 0, rect.height, colour);
		port->drawVertLine((rect.width + FLOW_SIZE) / 2, 0, rect.height, colour);
	}
};
	
#endif