#ifndef _AMIGA_SCREEN_H_
#define _AMIGA_SCREEN_H_

#include <nds.h>
#include "eventhandler.h"
#include "screen.h"

using namespace std;

class DecorationGlyphButton;
class ScreenFlipButton;
class ScreenTitle;

class AmigaScreen : public Screen, public EventHandler {

public:
	AmigaScreen(char* title, FontBase* font = NULL);
	virtual ~AmigaScreen();

	virtual void setBorderless(bool isBorderless);
	virtual bool handleEvent(const EventArgs& e);

protected:
	ScreenTitle* _screenTitle;
	DecorationGlyphButton* _depthButton;
	DecorationGlyphButton* _flipButton;
};

#endif
