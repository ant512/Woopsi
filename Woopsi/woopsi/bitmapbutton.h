#ifndef _BITMAP_BUTTON_H_
#define _BITMAP_BUTTON_H_

#include <nds.h>
#include "textbox.h"

using namespace std;

class BitmapButton : public Textbox {

public:
	BitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, u16 bitmapWidth, u16 bitmapHeight, const u16* bitmapNormal, const u16* bitmapClicked);

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	virtual void draw(Rect clipRect);

protected:
	const u16* _bitmapNormal;
	const u16* _bitmapClicked;
	u16 _bitmapWidth;
	u16 _bitmapHeight;
	u16 _bitmapX;
	u16 _bitmapY;

	/**
	 * Destructor.
	 */
	virtual inline ~BitmapButton() { };
};

#endif
