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
	
	void drawSparkline(GraphicsPort* port, const Rect& rect) {
		s16 midX = rect.width / 2;
		s16 midY = rect.height / 2;
		
		s16 bottomX = midX;
		s16 bottomY = rect.height;
		
		s16 rightX = rect.width;
		s16 rightY = midY;
		
		s16 startX = 0;
		s16 startY = 0;
		s16 endX = 0;
		s16 endY = 0;
		
		if (!hasAvailableBottomConnector()) {
			
			// Flow is moving from bottom to right
			startX = bottomX;
			startY = bottomY;
			
			endX = rightX;
			endY = rightY;
		} else {
			
			// Flow is moving from right to bottom
			startX = rightX;
			startY = rightY;
			
			endX = bottomX;
			endY = bottomY;
		}
		
		s16 burnSize = (getFlowLevel() * getWidth()) / 100;
		
		port->drawLine(startX, startY, midX, midY, woopsiRGB(0, 0, 0));
		port->drawLine(midX, midY, endX, endY, woopsiRGB(0, 0, 0));
		
		if (getFlowLevel() > 0) {
			if (getFlowLevel() < MAX_PIPE_BUTTON_FLOW / 2) {
				if (startX == midX) {
					port->drawLine(startX, startY, midX, startY + burnSize, woopsiRGB(31, 0, 0));
					drawSpark(startX, startY + burnSize, port);
				} else {
					port->drawLine(startX, startY, startX - burnSize, midY, woopsiRGB(31, 0, 0));
					drawSpark(startX - burnSize, startY, port);
				}
			} else {
				port->drawLine(startX, startY, midX, midY, woopsiRGB(31, 0, 0));
				
				// Half of the burn is now used up
				burnSize = ((getFlowLevel() - (MAX_PIPE_BUTTON_FLOW / 2)) * getWidth()) / 100;
				
				if (endX == midX) {
					port->drawLine(midX, midY, endX, midY - burnSize, woopsiRGB(31, 0, 0));
					
					if (getFlowLevel() < MAX_PIPE_BUTTON_FLOW) {
						drawSpark(midX, midY - burnSize, port);
					}
				} else {
					port->drawLine(midX, midY, midX + burnSize, endY, woopsiRGB(31, 0, 0));
					
					if (getFlowLevel() < MAX_PIPE_BUTTON_FLOW) {
						drawSpark(midX + burnSize, midY, port);
					}
				}
			}
		}
	};
	
	void drawContents(GraphicsPort* port) {

		// Do not draw if contents not revealed
		if (!isRevealed()) {
			drawUnrevealedContents(port);
			return;
		}
		
		Rect rect;
		getClientRect(rect);
		drawSparkline(port, rect);
	};
};
	
#endif