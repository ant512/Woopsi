#ifndef _SKINNED_WINDOW_BORDER_BOTTOM_H_
#define _SKINNED_WINDOW_BORDER_BOTTOM_H_

#include <nds.h>
#include "windowborderbottom.h"
#include "skin.h"

using namespace std;

class SkinnedWindowBorderBottom : public WindowBorderBottom {

public:
	SkinnedWindowBorderBottom(s16 x, s16 y, u16 width, const WindowSkin* skin);
	virtual ~SkinnedWindowBorderBottom();

	virtual void draw(Rect clipRect);
	virtual void draw();

protected:
	const WindowSkin* _skin;
};

#endif
