#ifndef _SKINNED_SCREEN_TITLE_H_
#define _SKINNED_SCREEN_TITLE_H_

#include <nds.h>
#include "screentitle.h"
#include "skin.h"

using namespace std;

class SkinnedScreenTitle : public ScreenTitle {

public:
	SkinnedScreenTitle(char* text, const ScreenSkin* skin);

	virtual void draw(Rect clipRect);

protected:
	const ScreenSkin* _skin;

	/**
	 * Destructor.
	 */
	virtual inline ~SkinnedScreenTitle() { };
};

#endif
