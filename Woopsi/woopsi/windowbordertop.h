#ifndef _WINDOW_BORDER_TOP_H_
#define _WINDOW_BORDER_TOP_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

class WindowBorderTop : public Gadget {

public:
	WindowBorderTop(s16 x, u16 width, u16 height, char* text, FontBase* font = NULL);

	virtual void draw(Rect clipRect);
	virtual void draw();

	virtual bool focus();
	virtual bool blur();

	virtual bool click(s16 x, s16 y);

protected:
	char* _text;

	/**
	 * Destructor.
	 */
	virtual inline ~WindowBorderTop() {
		if (_text != NULL) {
			delete [] _text;
		}
	};
};

#endif
