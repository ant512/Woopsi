#ifndef _ANIM_BUTTON_H_
#define _ANIM_BUTTON_H_

#include <nds.h>
#include "textbox.h"
#include "graphicsport.h"
#include "animation.h"

using namespace std;

class AnimButton : public Textbox {

public:
	AnimButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, u16 bitmapWidth, u16 bitmapHeight);
	virtual ~AnimButton();
	virtual void draw(Rect clipRect);

	virtual Animation* const getNormalAnimation();
	virtual Animation* const getClickedAnimation();
	bool vbl();
	bool click(s16 x, s16 y);
	bool release(s16 x, s16 y);

protected:
	Animation* _animNormal;
	Animation* _animClicked;
	u16 _bitmapWidth;
	u16 _bitmapHeight;
	u16 _bitmapX;
	u16 _bitmapY;
	bool _initialised;
};

#endif
