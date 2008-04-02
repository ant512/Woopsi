#ifndef _SKINNED_WINDOW_BORDER_LEFT_H_
#define _SKINNED_WINDOW_BORDER_LEFT_H_

#include <nds.h>
#include "windowborderside.h"
#include "skin.h"

using namespace std;

class SkinnedWindowBorderLeft : public WindowBorderSide {
public:
	SkinnedWindowBorderLeft(s16 x, s16 y, u16 height, const WindowSkin* skin);

	virtual void draw(Rect clipRect);
	virtual void draw();

protected:
	const WindowSkin* _skin;

	/**
	 * Destructor.
	 */
	virtual inline ~SkinnedWindowBorderLeft() { };
};

#endif
