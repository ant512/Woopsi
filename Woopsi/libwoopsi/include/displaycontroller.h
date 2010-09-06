#ifndef _DISPLAY_CONTROLLER_H_
#define _DISPLAY_CONTROLLER_H_

#include "woopsiarray.h"
#include "rect.h"

namespace WoopsiUI {

	class Gadget;

	class DisplayController {
	public:
		DisplayController(Gadget* gadget);
		~DisplayController();

		void addDamagedRect(const Rect& rect);
		void redraw(Gadget* gadget = NULL);

	private:
		WoopsiArray<Rect> _damagedRects;
		Gadget* _gadget;
	};
}

#endif
