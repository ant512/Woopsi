#ifndef _SCREEN_BASE_H_
#define _SCREEN_BASE_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

class Screen : public Gadget {

public:
	Screen(char* title, FontBase* font = NULL);
	
	virtual void getClientRect(Rect& rect) const;

	virtual inline const u8 getTitleHeight() const { return _titleHeight; };
	virtual inline const char* getTitle() const { return _title; };

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	virtual void draw(Rect clipRect);

	virtual void setActiveGadget(Gadget* gadget);

	virtual bool swapGadgetDepth(Gadget* gadget);

	virtual bool release(s16 x, s16 y);
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual void flipToTopScreen();
	virtual void flipToBottomScreen();
	virtual bool flipScreens();

protected:
	u8 _titleHeight;
	char* _title;

	void setActive(bool active);

	/**
	 * Destructor.
	 */
	virtual inline ~Screen() { };
};

#endif
