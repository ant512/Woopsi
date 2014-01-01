#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "window.h"
#include "gadgeteventhandler.h"
#include "superbitmap.h"
#include "graphics.h"

using namespace WoopsiUI;

class Canvas : public Window, public GadgetEventHandler {
public:

	enum CanvasMode {
		CANVAS_MODE_DOTTED_DRAW = 0,
		CANVAS_MODE_SOLID_DRAW = 1,
		CANVAS_MODE_LINE = 2,
		CANVAS_MODE_RECT = 3,
		CANVAS_MODE_FILLED_RECT = 4,
		CANVAS_MODE_ELLIPSE = 5,
		CANVAS_MODE_FILLED_ELLIPSE = 6,
		CANVAS_MODE_FLOOD_FILL = 7
	};

	Canvas() : Window(0, 0, 256, 192, "Canvas Window") {
		_flags.decoration = true;
		_superBitmap = new SuperBitmap(0, 0, 256, 192, 256, 192, 1);
		_superBitmap->setDraggable(true);
		_superBitmap->setGadgetEventHandler(this);
		_graphics = _superBitmap->getGraphics();
		addGadget(_superBitmap);
		
		_mode = CANVAS_MODE_FILLED_ELLIPSE;
		_foregroundColour = woopsiRGB(31, 0, 0);
		_backgroundColour = woopsiRGB(0, 0, 0);
		
		_graphics->drawFilledRect(0, 0, 256, 192, _backgroundColour);
	};
	
	~Canvas() { };
	
	void setMode(CanvasMode mode) { _mode = mode; };
	
	void setColour(u16 colour) { _foregroundColour = colour; };
	
	void handleClickEvent(Gadget& source, const WoopsiPoint& point) {
		switch(_mode) {
			case CANVAS_MODE_DOTTED_DRAW:
			case CANVAS_MODE_SOLID_DRAW:
				_graphics->drawPixel(point.getX(), point.getY(), _foregroundColour);
				break;
				
			case CANVAS_MODE_LINE:
				_graphics->drawLine(point.getX() - (rand() % 20), point.getY() - (rand() % 20), point.getX() + (rand() % 20), point.getY() + (rand() % 20), _foregroundColour);
				break;
				
			case CANVAS_MODE_RECT:
				_graphics->drawRect(point.getX(), point.getY(), 30, 30, _foregroundColour);
				break;
				
			case CANVAS_MODE_FILLED_RECT:
				_graphics->drawFilledRect(point.getX(), point.getY(), 30, 30, _foregroundColour);
				break;
				
			case CANVAS_MODE_ELLIPSE:
				_graphics->drawEllipse(point.getX(), point.getY(), 30, 20, _foregroundColour);
				break;
				
			case CANVAS_MODE_FILLED_ELLIPSE:
				_graphics->drawFilledEllipse(point.getX(), point.getY(), 30, 20, _foregroundColour);
				break;
				
			case CANVAS_MODE_FLOOD_FILL:
				_graphics->floodFill(point.getX(), point.getY(), _foregroundColour);
				break;
				
			default:
				break;
		}

		_superBitmap->markRectsDamaged();
		_oldStylusX = point.getX();
		_oldStylusY = point.getY();
	};
	
	void handleDragEvent(Gadget& source, const WoopsiPoint& point, const WoopsiPoint& delta) {
		switch(_mode) {
			case CANVAS_MODE_DOTTED_DRAW:
				_graphics->drawPixel(point.getX(), point.getY(), _foregroundColour);
				break;
			
			case CANVAS_MODE_SOLID_DRAW:
				_graphics->drawLine(_oldStylusX, _oldStylusY, point.getX(), point.getY(), _foregroundColour);
				break;
				
			default:
				break;
		}
		
		_superBitmap->markRectsDamaged();
		_oldStylusX = point.getX();
		_oldStylusY = point.getY();
	};
	
	bool drag(s16 x, s16 y, s16 vX, s16 vY) {
		switch(_mode) {
			case CANVAS_MODE_DOTTED_DRAW:
				_graphics->drawPixel(x, y, _foregroundColour);
				break;
			
			case CANVAS_MODE_SOLID_DRAW:
				_graphics->drawLine(_oldStylusX, _oldStylusY, x, y, _foregroundColour);
				break;
			
			default:
				break;
		}

		_superBitmap->markRectsDamaged();
		_oldStylusX = x;
		_oldStylusY = y;
		
		return true;
	};
		
private:
	SuperBitmap* _superBitmap;
	Graphics* _graphics;
	s16 _oldStylusX;
	s16 _oldStylusY;
	CanvasMode _mode;
	u16 _foregroundColour;
	u16 _backgroundColour;
};

#endif
