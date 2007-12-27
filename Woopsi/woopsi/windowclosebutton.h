#ifndef _WINDOW_CLOSE_BUTTON_H_
#define _WINDOW_CLOSE_BUTTON_H_

#include <nds.h>
#include "textwriter.h"
#include "button.h"
#include "graphicsport.h"

using namespace std;

class WindowCloseButton : public Button {

public:
	WindowCloseButton(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);
	virtual ~WindowCloseButton();

	virtual void draw(Rect clipRect);
	virtual void draw();

	virtual bool release(s16 x, s16 y);
	virtual bool focus();
	virtual bool blur();
};

#endif
