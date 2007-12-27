#ifndef _SCREEN_DEPTH_BUTTON_H_
#define _SCREEN_DEPTH_BUTTON_H_

#include <nds.h>
#include "textwriter.h"
#include "button.h"
#include "screen.h"

using namespace std;

class ScreenDepthButton : public Button {

public:
	ScreenDepthButton(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);
	~ScreenDepthButton();

	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);
};

#endif
