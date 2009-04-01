#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "window.h"
#include "gadgeteventhandler.h"
#include "superbitmap.h"

using namespace WoopsiUI;

class Canvas : public Window, public GadgetEventHandler {
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
		_superBitmap->addGadgetEventHandler(this);
		addGadget(_superBitmap);
		
		_mode = CANVAS_MODE_FILLED_CIRCLE;
		_foregroundColour = woopsiRGB(31, 0, 0);
		_backgroundColour = woopsiRGB(0, 0, 0);
		
		_superBitmap->drawFilledRect(0, 0, 256, 192, _backgroundColour);
	};
	
	~Canvas() { };
	
	void setMode(CanvasMode mode) { _mode = mode; };
	
	void setColour(u16 colour) { _foregroundColour = colour; };
	
	void handleClickEvent(const GadgetEventArgs& e) {
		switch(_mode) {
			case CANVAS_MODE_DOTTED_DRAW:
			case CANVAS_MODE_SOLID_DRAW:
				_superBitmap->drawPixel(e.getX(), e.getY(), _foregroundColour);
				break;
				
			case CANVAS_MODE_LINE:
				_superBitmap->drawLine(e.getX() - (rand() % 20), e.getY() - (rand() % 20), e.getX() + (rand() % 20), e.getY() + (rand() % 20), _foregroundColour);
				break;
			
			case CANVAS_MODE_CIRCLE:
				_superBitmap->drawCircle(e.getX(), e.getY(), 30, _foregroundColour);
				break;
				
			case CANVAS_MODE_FILLED_CIRCLE:
				_superBitmap->drawFilledCircle(e.getX(), e.getY(), 30, _foregroundColour);
				break;
				
			case CANVAS_MODE_RECT:
				_superBitmap->drawRect(e.getX(), e.getY(), 30, 30, _foregroundColour);
				break;
				
			case CANVAS_MODE_FILLED_RECT:
				_superBitmap->drawFilledRect(e.getX(), e.getY(), 30, 30, _foregroundColour);
				break;
				
			case CANVAS_MODE_ELLIPSE:
				_superBitmap->drawEllipse(e.getX(), e.getY(), 30, 20, _foregroundColour);
				break;
				
			case CANVAS_MODE_FILLED_ELLIPSE:
				_superBitmap->drawFilledEllipse(e.getX(), e.getY(), 30, 20, _foregroundColour);
				break;
				
			case CANVAS_MODE_FLOOD_FILL:
				_superBitmap->floodFill(e.getX(), e.getY(), _foregroundColour);
				break;
				
			default:
				break;
		}

		_superBitmap->draw();
		_oldStylusX = e.getX();
		_oldStylusY = e.getY();
	};
	
	void handleDragEvent(const GadgetEventArgs& e) {
		switch(_mode) {
			case CANVAS_MODE_DOTTED_DRAW:
				_superBitmap->drawPixel(e.getX(), e.getY(), _foregroundColour);
				break;
			
			case CANVAS_MODE_SOLID_DRAW:
				_superBitmap->drawLine(_oldStylusX, _oldStylusY, e.getX(), e.getY(), _foregroundColour);
				break;
				
			default:
				break;
		}
		
		_superBitmap->draw();
		_oldStylusX = e.getX();
		_oldStylusY = e.getY();
	};
	
	bool drag(s16 x, s16 y, s16 vX, s16 vY) {
		switch(_mode) {
			case CANVAS_MODE_DOTTED_DRAW:
				_superBitmap->drawPixel(x, y, _foregroundColour);
				break;
			
			case CANVAS_MODE_SOLID_DRAW:
				_superBitmap->drawLine(_oldStylusX, _oldStylusY, x, y, _foregroundColour);
				break;
			
			default:
				break;
		}

		_superBitmap->draw();
		_oldStylusX = x;
		_oldStylusY = y;
		
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
