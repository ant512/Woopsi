#ifndef _WINDOW_DEPTH_BUTTON_H_
#define _WINDOW_DEPTH_BUTTON_H_

#include <nds.h>
#include "button.h"

using namespace std;

class WindowDepthButton : public Button {

public:
	WindowDepthButton(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);
	virtual ~WindowDepthButton();

	virtual void draw(Rect clipRect);
	virtual void draw();

	virtual bool release(s16 x, s16 y);
	virtual bool focus();
	virtual bool blur();
};

#endif
