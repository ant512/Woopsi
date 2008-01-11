#ifndef _AMIGA_SCREEN_H_
#define _AMIGA_SCREEN_H_

#include <nds.h>
#include <vector>
#include "gadget.h"
#include "screentitle.h"
#include "fontbase.h"
#include "screen.h"

using namespace std;

class ScreenDepthButton;
class ScreenFlipButton;

class AmigaScreen : public Screen {

public:
	AmigaScreen(char* title, FontBase* font = NULL);
	virtual ~AmigaScreen();

	virtual void setBorderless(bool isBorderless);
	virtual bool handleEvent(const EventArgs& e);

protected:
	ScreenTitle* _screenTitle;
	ScreenDepthButton* _screenDepthButton;
	ScreenFlipButton* _screenFlipButton;
};

#endif
