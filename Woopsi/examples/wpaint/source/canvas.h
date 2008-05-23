#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "window.h"
#include "eventhandler.h"
#include "superbitmap.h"

using namespace std;

class Canvas : public Window, public EventHandler {
public:

	enum CanvasMode {
		CANVAS_MODE_DOTTED_DRAW = 0,
		CANVAS_MODE_SOLID_DRAW = 1,
		CANVAS_MODE_LINE = 2,
		CANVAS_MODE_RECT = 3,
		CANVAS_MODE_FILLED_RECT = 4,
		CANVAS_MODE_CIRCLE = 5,
		CANVAS_MODE_FILLED_CIRCLE = 6,
		CANVAS_MODE_ELLIPSE = 7,
		CANVAS_MODE_FILLED_ELLIPSE = 8,
		CANVAS_MODE_FLOOD_FILL = 9
	};

	Canvas() : Window(0, 0, 256, 192, "Canvas Window", GADGET_DECORATION) {	
		_superBitmap = new SuperBitmap(0, 0, 256, 192, 256, 192, 1);
		_superBitmap->setDraggable(true);
		_superBitmap->setEventHandler(this);
		addGadget(_superBitmap);
		
		_mode = CANVAS_MODE_FILLED_CIRCLE;
		_foregroundColour = woopsiRGB(31, 0, 0);
		_backgroundColour = woopsiRGB(0, 0, 0);
		
		_superBitmap->drawFilledRect(0, 0, 256, 192, _backgroundColour);
	};
	
	~Canvas() { };
	
	void setMode(CanvasMode mode) { _mode = mode; };
	
	void setColour(u16 colour) { _foregroundColour = colour; };
	
	bool handleEvent(const EventArgs& e) {
		switch(e.type) {
			case EVENT_CLICK:
				switch(_mode) {
					case CANVAS_MODE_DOTTED_DRAW:
					case CANVAS_MODE_SOLID_DRAW:
						_superBitmap->drawPixel(e.eventX, e.eventY, _foregroundColour);
						break;
						
					case CANVAS_MODE_LINE:
						_superBitmap->drawLine(e.eventX - (rand() % 20), e.eventY - (rand() % 20), e.eventX + (rand() % 20), e.eventY + (rand() % 20), _foregroundColour);
						break;
					
					case CANVAS_MODE_CIRCLE:
						_superBitmap->drawCircle(e.eventX, e.eventY, 30, _foregroundColour);
						break;
						
					case CANVAS_MODE_FILLED_CIRCLE:
						_superBitmap->drawFilledCircle(e.eventX, e.eventY, 30, _foregroundColour);
						break;
						
					case CANVAS_MODE_RECT:
						_superBitmap->drawRect(e.eventX, e.eventY, 30, 30, _foregroundColour);
						break;
						
					case CANVAS_MODE_FILLED_RECT:
						_superBitmap->drawFilledRect(e.eventX, e.eventY, 30, 30, _foregroundColour);
						break;
						
					case CANVAS_MODE_ELLIPSE:
						_superBitmap->drawEllipse(e.eventX, e.eventY, 30, 20, _foregroundColour);
						break;
						
					case CANVAS_MODE_FILLED_ELLIPSE:
						_superBitmap->drawFilledEllipse(e.eventX, e.eventY, 30, 20, _foregroundColour);
						break;
						
					case CANVAS_MODE_FLOOD_FILL:
						_superBitmap->floodFill(e.eventX, e.eventY, _foregroundColour);
						break;
						
					default:
						break;
				}
				
				_superBitmap->draw();
				_oldStylusX = e.eventX;
				_oldStylusY = e.eventY;
				break;
			case EVENT_DRAG:
				switch(_mode) {
					case CANVAS_MODE_DOTTED_DRAW:
						_superBitmap->drawPixel(e.eventX, e.eventY, _foregroundColour);
						break;
					
					case CANVAS_MODE_SOLID_DRAW:
						_superBitmap->drawLine(_oldStylusX, _oldStylusY, e.eventX, e.eventY, _foregroundColour);
						break;
						
					default:
						break;
				}
				
				_superBitmap->draw();
				_oldStylusX = e.eventX;
				_oldStylusY = e.eventY;
				break;
			default:
				break;
		}
		
		return true;
	};
		
private:
	SuperBitmap* _superBitmap;
	s16 _oldStylusX;
	s16 _oldStylusY;
	CanvasMode _mode;
	u16 _foregroundColour;
	u16 _backgroundColour;
};

#endif
