#ifndef _GRAPHICS_PORT_H_
#define _GRAPHICS_PORT_H_

#include <nds.h>
#include "debug.h"
#include "defines.h"
#include "gadget.h"
#include "fontbase.h"
#include "textwriter.h"
#include "woopsifuncs.h"

using namespace std;

class GraphicsPort {
public:
	GraphicsPort(Gadget* const gadget, const s16 x, const s16 y, const u16 width, const u16 height, u16* const bitmap, const u16 bitmapWidth, const u16 bitmapHeight, const Gadget::Rect* clipRect = NULL);
	~GraphicsPort();

	s16 getX();
	s16 getY();

	// Externally-visible drawing functions
	void drawPixel(u16 x, u16 y, u16 colour);
	void drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
	void drawHorizLine(s16 x, s16 y, s16 width, u16 colour);
	void drawVertLine(s16 x, s16 y, s16 height, u16 colour);
	void drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
	void drawBevelledRect(s16 x, s16 y, u16 width, u16 height, u16 shineColour, u16 shadowColour);
	void drawBevelledRect(s16 x, s16 y, u16 width, u16 height);
	void drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour);
	void drawXORPixel(s16 x, s16 y);
	void drawXORHorizLine(s16 x, s16 y, s16 width);
	void drawXORVertLine(s16 x, s16 y, s16 height);
	void drawXORRect(s16 x, s16 y, u16 width, u16 height);
	void drawText(s16 x, s16 y, FontBase* font, char* string);
	void drawText(s16 x, s16 y, FontBase* font, u16 length, char* string);
	void drawText(s16 x, s16 y, FontBase* font, char letter);
	void drawText(s16 x, s16 y, FontBase* font, char* string, u16 colour);
	void drawText(s16 x, s16 y, FontBase* font, char letter, u16 colour);
	void drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight);
	void clear();

private:
	Gadget* _gadget;
	Gadget::Rect* _clipRect;
	Gadget::Rect _rect;
	u16* _bitmap;
	u16 _bitmapWidth;
	u16 _bitmapHeight;

	// Internal clipping routines
	void clipPixel(u16 x, u16 y, u16 colour, const Gadget::Rect& clipRect);
	void clipFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour, const Gadget::Rect& clipRect);
	void clipHorizLine(s16 x, s16 y, s16 width, u16 colour, const Gadget::Rect& clipRect);
	void clipVertLine(s16 x, s16 y, s16 height, u16 colour, const Gadget::Rect& clipRect);
	void clipBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight, const Gadget::Rect& clipRect);
	void clipText(s16 x, s16 y, FontBase* font, u16 length, char* string, const Gadget::Rect& clipRect);
	void clipFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour, const Gadget::Rect& clipRect);
	void clipXORPixel(s16 x, s16 y, const Gadget::Rect& clipRect);
	void clipXORHorizLine(s16 x, s16 y, s16 width, const Gadget::Rect& clipRect);
	void clipXORVertLine(s16 x, s16 y, s16 height, const Gadget::Rect& clipRect);

	// Drawing functions that take pre-clipped values
	void drawClippedPixel(u16 x, u16 y, u16 colour);
	void drawClippedFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
	void drawClippedHorizLine(s16 x, s16 y, s16 width, u16 colour);
	void drawClippedVertLine(s16 x, s16 y, s16 height, u16 colour);
	void drawClippedBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight);
	void drawClippedXORPixel(s16 x, s16 y);
	void drawClippedXORHorizLine(s16 x, s16 y, u16 width);
	void drawClippedXORVertLine(s16 x, s16 y, u16 height);
	void drawClippedXORRect(s16 x, s16 y, u16 width, u16 height);

	void convertPortToScreenSpace(s16* x, s16* y);
	bool clipCoordinates(s16* x1, s16* y1, s16* x2, s16* y2, const Gadget::Rect& clipRect);
};

#endif
