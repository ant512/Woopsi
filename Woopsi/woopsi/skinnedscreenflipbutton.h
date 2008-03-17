#ifndef _SKINNED_SCREEN_FLIP_BUTTON_H_
#define _SKINNED_SCREEN_FLIP_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"
#include "skin.h"

using namespace std;

class SkinnedScreenFlipButton : public DecorationGlyphButton {

public:
	SkinnedScreenFlipButton(s16 x, s16 y, const ScreenSkin* skin);
	~SkinnedScreenFlipButton();

	virtual void draw(Rect clipRect);

protected:
	const ScreenSkin* _skin;
};

#endif
