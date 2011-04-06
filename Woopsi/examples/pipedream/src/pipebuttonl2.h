#ifndef _PIPE_BUTTON_L2_H_
#define _PIPE_BUTTON_L2_H_

#include "pipebuttonstandard.h"
#include "graphicsport.h"
#include "constants.h"

using namespace WoopsiUI;

class PipeButtonL2 : public PipeButtonStandard {
public:
	PipeButtonL2(s16 x, s16 y, u16 width, u16 height) : PipeButtonStandard(x, y, width, height, false, true, true, false) { };
	
protected:
	virtual ~PipeButtonL2() { };
	
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
		if (!hasAvailableBottomConnector()) {
		
			// Flow is moving from bottom to right
			if (getFlowLevel() > MAX_PIPE_BUTTON_FLOW / 2) {
			
				// Fill entire vertical section
				port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, (rect.height - FLOW_SIZE) / 2, FLOW_SIZE, (rect.height - FLOW_SIZE) / 2 + FLOW_SIZE, woopsiRGB(0, 0, 20));
				
				// Draw horizontal section
				port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, ((rect.height - FLOW_SIZE) / 2) + 1, getFlowLevel() - ((rect.width - FLOW_SIZE) / 2) + 1, FLOW_SIZE, woopsiRGB(0, 0, 20));
			} else {
			
				// Draw part of vertical section
				port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, (rect.height - FLOW_SIZE) / 2, FLOW_SIZE, getFlowLevel(), woopsiRGB(0, 0, 20));
			}
		} else {
		
			// Flow is moving from right to bottom
			if (getFlowLevel() > MAX_PIPE_BUTTON_FLOW / 2) {
			
				// Fill entire horizontal section
				port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, ((rect.height - FLOW_SIZE) / 2) + 1, ((rect.width - FLOW_SIZE) / 2) + 1, FLOW_SIZE, woopsiRGB(0, 0, 20));
				
				// Draw part of vertical section
				port->drawFilledRect(((rect.width - FLOW_SIZE) / 2) + 1, (rect.height - FLOW_SIZE) / 2, FLOW_SIZE, (rect.height - FLOW_SIZE) / 2 + FLOW_SIZE, woopsiRGB(0, 0, 20));
			} else {
			
				// Draw part of horizontal section
			}
		}
		
		s16 x1 = (rect.width - FLOW_SIZE) / 2;
		s16 y1 = (rect.height - FLOW_SIZE) / 2;
		s16 x2 = x1;
		s16 y2 = y1 + ((rect.height + FLOW_SIZE) / 2) - 1;
		
		port->drawLine(x1, y1, x2, y2, colour);
		
		x1 = (rect.width + FLOW_SIZE) / 2;
		y1 = (rect.height + FLOW_SIZE) / 2;
		x2 = x1;
		y2 = y1 + ((rect.height - FLOW_SIZE) / 2) - 1;
		
		port->drawLine(x1, y1, x2, y2, colour);
		
		x1 = (rect.width - FLOW_SIZE) / 2;
		y1 = (rect.height - FLOW_SIZE) / 2;
		x2 = x1 + ((rect.width + FLOW_SIZE) / 2) - 1;
		y2 = y1;

		port->drawLine(x1, y1, x2, y2, colour);

		x1 = (rect.width + FLOW_SIZE) / 2;
		y1 = (rect.height + FLOW_SIZE) / 2;
		x2 = x1 + ((rect.width - FLOW_SIZE) / 2) - 1;
		y2 = y1;
		
		port->drawLine(x1, y1, x2, y2, colour);
	};
};
	
#endif