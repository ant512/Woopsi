#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

#include "window.h"
#include "eventhandler.h"
#include "button.h"
#include "canvas.h"

using namespace std;

class Toolbox : public Window, public EventHandler {
public:
	Toolbox(Canvas* canvas) : Window(244, 0, 12, 192, "Toolbox", 0) {
		_canvas = canvas;
	
		Button* b;
		
		// Create drawing tool selection buttons
		for (u8 i = 0; i < 10; i++) {
			b = new Button(0, i * 12, 12, 12, 65 + i);
			b->setRefcon(i + 1);
			b->setEventHandler(this);
			addGadget(b);
		}
		
		// Create colour buttons
		b = new Button(0, 120, 12, 12, "");
		b->setRefcon(11);
		b->setEventHandler(this);
		b->setBackColour(woopsiRGB(31, 0, 0));
		addGadget(b);
		
		b = new Button(0, 132, 12, 12, "");
		b->setRefcon(12);
		b->setEventHandler(this);
		b->setBackColour(woopsiRGB(0, 31, 0));
		addGadget(b);
		
		b = new Button(0, 144, 12, 12, "");
		b->setRefcon(13);
		b->setEventHandler(this);
		b->setBackColour(woopsiRGB(0, 0, 31));
		addGadget(b);
		
		b = new Button(0, 156, 12, 12, "");
		b->setRefcon(13);
		b->setEventHandler(this);
		b->setBackColour(woopsiRGB(31, 31, 0));
		addGadget(b);
		
		b = new Button(0, 168, 12, 12, "");
		b->setRefcon(14);
		b->setEventHandler(this);
		b->setBackColour(woopsiRGB(0, 31, 31));
		addGadget(b);
		
		b = new Button(0, 180, 12, 12, "");
		b->setRefcon(15);
		b->setEventHandler(this);
		b->setBackColour(woopsiRGB(31, 31, 31));
		addGadget(b);
	};
	
	bool handleEvent(const EventArgs& e) {
		switch(e.gadget->getRefcon()) {
			case 1:
				_canvas->setMode(Canvas::CANVAS_MODE_DOTTED_DRAW);
				break;
			case 2:
				_canvas->setMode(Canvas::CANVAS_MODE_SOLID_DRAW);
				break;
			case 3:
				_canvas->setMode(Canvas::CANVAS_MODE_LINE);
				break;
			case 4:
				_canvas->setMode(Canvas::CANVAS_MODE_RECT);
				break;
			case 5:
				_canvas->setMode(Canvas::CANVAS_MODE_FILLED_RECT);
				break;
			case 6:
				_canvas->setMode(Canvas::CANVAS_MODE_CIRCLE);
				break;
			case 7:
				_canvas->setMode(Canvas::CANVAS_MODE_FILLED_CIRCLE);
				break;
			case 8:
				_canvas->setMode(Canvas::CANVAS_MODE_ELLIPSE);
				break;
			case 9:
				_canvas->setMode(Canvas::CANVAS_MODE_FILLED_ELLIPSE);
				break;
			case 10:
				_canvas->setMode(Canvas::CANVAS_MODE_FLOOD_FILL);
				break;
			default:
				// Colours
				_canvas->setColour(e.gadget->getBackColour());
				break;
			}
		return true;
	};

private:
	Canvas* _canvas;
};

#endif
