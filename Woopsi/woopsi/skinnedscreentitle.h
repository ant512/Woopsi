#ifndef _SKINNED_SCREEN_TITLE_H_
#define _SKINNED_SCREEN_TITLE_H_

#include <nds.h>
#include "graphicsport.h"
#include "screentitle.h"
#include "skin.h"

using namespace std;

class SkinnedScreenTitle : public ScreenTitle {

public:
	SkinnedScreenTitle(char* text, const ScreenSkin* skin);
	~SkinnedScreenTitle();

	virtual void draw(Rect clipRect);

protected:
	const ScreenSkin* _skin;
};

#endif
