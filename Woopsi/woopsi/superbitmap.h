#ifndef _SUPERBITMAP_H_
#define _SUPERBITMAP_H_

#include <nds.h>
#include <vector>
#include "textwriter.h"
#include "gadget.h"
#include "graphicsport.h"

using namespace std;

class SuperBitmap : public Gadget {

public:
	SuperBitmap(s16 x, s16 y, u16 width, u16 height, u16 bitmapWidth, u16 bitmapHeight, bool isDecoration, FontBase* font = NULL);
	virtual ~SuperBitmap();

	virtual void initBitmap();

	u16 getPixel(s16 x, s16 y);
	u16* getBitmap();

	void setAllowStylusScroll(bool allowStylusScroll);

	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual void drawPixel(s16 x, s16 y, u16 colour);
	virtual void drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
	virtual void drawHorizLine(s16 x, s16 y, u16 width, u16 colour);
	virtual void drawVertLine(s16 x, s16 y, u16 height, u16 colour);
	virtual void drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
	virtual void drawLine(s16 x1, s16 y1, u16 x2, u16 y2, u16 colour);
	virtual void drawCircle(s16 x0, s16 y0, u16 radius, u16 colour);
	virtual void drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour);
	virtual void drawText(s16 x, s16 y, FontBase* font, char* string);
	virtual void drawText(s16 x, s16 y, FontBase* font, char* string, u16);
	virtual void drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight);
	virtual void floodFill(s16 x, s16 y, u16 newColour);

	virtual void clearBitmap();

	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

protected:
	TextWriter* _textWriter;
	s32 _bitmapX;
	s32 _bitmapY;
	u16 _bitmapWidth;
	u16 _bitmapHeight;
	u16* _bitmap __attribute__ ((aligned (4)));
	bool _allowStylusScroll;

	bool popStack(s16* x, s16* y, vector<u16>* stack);
	void pushStack(s16 x, s16 y, vector<u16>* stack) ;
	bool clipBitmapCoordinates(s16* x, s16* y, u16* width, u16* height);
};

#endif
