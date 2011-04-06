#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

#include "window.h"
#include "gadgeteventhandler.h"
#include "button.h"
#include "canvas.h"

using namespace WoopsiUI;

class Toolbox : public Window, public GadgetEventHandler {
public:
	Toolbox(Canvas* canvas) : Window(244, 0, 12, 192, "Toolbox", 0) {
		setBorderless(true);
		_canvas = canvas;
	
		Button* b;
		
		// Create drawing tool selection buttons
		for (u8 i = 0; i < 8; i++) {
			b = new Button(0, i * 14, 14, 14, 65 + i);
			b->setRefcon(i + 1);
			b->addGadgetEventHandler(this);
			addGadget(b);
		}
		
		// Create colour buttons
		b = new Button(0, 112, 14, 14, "");
		b->setRefcon(11);
		b->addGadgetEventHandler(this);
		b->setBackColour(woopsiRGB(31, 0, 0));
		addGadget(b);
		
		b = new Button(0, 126, 14, 14, "");
		b->setRefcon(12);
		b->addGadgetEventHandler(this);
		b->setBackColour(woopsiRGB(0, 31, 0));
		addGadget(b);
		
		b = new Button(0, 140, 14, 14, "");
		b->setRefcon(13);
		b->addGadgetEventHandler(this);
		b->setBackColour(woopsiRGB(0, 0, 31));
		addGadget(b);
		
		b = new Button(0, 154, 14, 14, "");
		b->setRefcon(13);
		b->addGadgetEventHandler(this);
		b->setBackColour(woopsiRGB(31, 31, 0));
		addGadget(b);
		
		b = new Button(0, 168, 14, 14, "");
		b->setRefcon(14);
		b->addGadgetEventHandler(this);
		b->setBackColour(woopsiRGB(0, 31, 31));
		addGadget(b);
		
		b = new Button(0, 182, 14, 14, "");
		b->setRefcon(15);
		b->addGadgetEventHandler(this);
		b->setBackColour(woopsiRGB(31, 31, 31));
		addGadget(b);
	};
	
	void handleActionEvent(const GadgetEventArgs& e) {
		switch(e.getSource()->getRefcon()) {
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
				_canvas->setMode(Canvas::CANVAS_MODE_ELLIPSE);
				break;
			case 7:
				_canvas->setMode(Canvas::CANVAS_MODE_FILLED_ELLIPSE);
				break;
			case 8:
				_canvas->setMode(Canvas::CANVAS_MODE_FLOOD_FILL);
				break;
			default:
				// Colours
				_canvas->setColour(e.getSource()->getBackColour());
				break;
		}
	};

private:
	Canvas* _canvas;
};

#endif
