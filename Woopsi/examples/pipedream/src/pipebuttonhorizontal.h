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
		if (!isRevealed()) {
			drawUnrevealedContents(port);
			return;
		}
		
		Rect rect;
		getClientRect(rect);
		
		s16 y1 = rect.height / 2;
		s16 y2 = y1;
		
		port->drawLine(0, y1, rect.width, y2, woopsiRGB(0, 0, 0));
		
		// Draw flow
		if (getFlowLevel() > 0) {
			s16 burntX1 = 0;
			s16 burntX2 = 0;
			
			if (!hasAvailableLeftConnector()) {
				
				// Flow is moving right
				burntX1 = 0;
				burntX2 = (rect.width * getFlowLevel()) / 100;
				
			} else {
				
				// Flow is moving left
				burntX1 = rect.width;
				burntX2 = rect.width - ((rect.width * getFlowLevel()) / 100);
			}
			
			port->drawLine(burntX1, y1, burntX2, y2, woopsiRGB(31, 0, 0));
			
			if (getFlowLevel() < 100) {
				drawSpark(burntX2, y2, port);
			}
		}
	}
};
	
#endif