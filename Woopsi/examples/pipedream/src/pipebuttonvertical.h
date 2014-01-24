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
		
		s16 x1 = rect.width / 2;
		s16 x2 = x1;
		
		port->drawLine(x1, 0, x2, rect.height, woopsiRGB(0, 0, 0));
		
		// Draw flow
		if (getFlowLevel() > 0) {
			s16 burntY1 = 0;
			s16 burntY2 = 0;
			
			if (!hasAvailableTopConnector()) {
			
				// Flow is moving downwards
				burntY1 = 0;
				burntY2 = (rect.height * getFlowLevel()) / 100;
				
			} else {
				
				// Flow is moving upwards
				burntY1 = rect.height;
				burntY2 = rect.height - ((rect.height * getFlowLevel()) / 100);
			}
		
			port->drawLine(x1, burntY1, x2, burntY2, woopsiRGB(31, 0, 0));
			
			if (getFlowLevel() < 100) {
				drawSpark(x1, burntY2, port);
			}
		}
	}
};
	
#endif