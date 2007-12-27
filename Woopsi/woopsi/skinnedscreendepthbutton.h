#ifndef _SKINNED_SCREEN_DEPTH_BUTTON_H_
#define _SKINNED_SCREEN_DEPTH_BUTTON_H_

#include <nds.h>
#include "screendepthbutton.h"
#include "skinnedscreen.h"
#include "skin.h"
#include "bitmap.h"

using namespace std;

class SkinnedScreenDepthButton : public ScreenDepthButton {

public:
	SkinnedScreenDepthButton(s16 x, s16 y, const ScreenSkin* skin);
	~SkinnedScreenDepthButton();

	virtual void draw(Rect clipRect);

protected:
	const ScreenSkin* _skin;
};

#endif
