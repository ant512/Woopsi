#ifndef _SKINNED_SCREEN_DEPTH_BUTTON_H_
#define _SKINNED_SCREEN_DEPTH_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"
#include "skin.h"

using namespace std;

class SkinnedScreenDepthButton : public DecorationGlyphButton {

public:
	SkinnedScreenDepthButton(s16 x, s16 y, const ScreenSkin* skin);

	virtual void draw(Rect clipRect);

protected:
	const ScreenSkin* _skin;

	/**
	 * Destructor.
	 */
	virtual inline ~SkinnedScreenDepthButton() { };
};

#endif
