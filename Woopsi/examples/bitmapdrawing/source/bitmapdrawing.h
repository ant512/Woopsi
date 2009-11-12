#ifndef _BITMAP_DRAWING_H_
#define _BITMAP_DRAWING_H_

#include "woopsi.h"
#include "superbitmap.h"
#include "gadgeteventhandler.h"
#include "gadgeteventargs.h"
#include "graphics.h"

using namespace WoopsiUI;

class BitmapDrawing : public Woopsi, public GadgetEventHandler {
public:

	enum DrawMode {
		DRAW_MODE_OFF,
		DRAW_MODE_LINE,
		DRAW_MODE_ELLIPSE,
		DRAW_MODE_FILLED_ELLIPSE,
		DRAW_MODE_RECT,
		DRAW_MODE_FILLED_RECT,
		DRAW_MODE_FILLED_XOR_RECT,
		DRAW_MODE_HORIZ_LINE,
		DRAW_MODE_VERT_LINE,
		DRAW_MODE_CIRCLE,
		DRAW_MODE_FILLED_CIRCLE,
		DRAW_MODE_XOR_HORIZ_LINE,
		DRAW_MODE_XOR_VERT_LINE,
		DRAW_MODE_XOR_RECT,
		DRAW_MODE_XOR_PIXEL,
		DRAW_MODE_PIXEL,
		DRAW_MODE_TEXT,
		DRAW_MODE_BITMAP,
		DRAW_MODE_BITMAP_TRANSPARENT,
		DRAW_MODE_FLOODFILL,
		DRAW_MODE_COPY,
		DRAW_MODE_DIM
	};

	void startup();
	void shutdown();
	
	void handleActionEvent(const GadgetEventArgs& e);

private:
	SuperBitmap* _superBitmap;
	Graphics* _graphics;
	DrawMode _mode;
	Bitmap* _bitmap;
	
	void handleTimer();
	
	void drawLine();
	void drawHorizLine();
	void drawVertLine();
	void drawEllipse();
	void drawFilledEllipse();
	void drawRect();
	void drawFilledRect();
	void drawFilledXORRect();
	void drawCircle();
	void drawFilledCircle();
	void drawXORHorizLine();
	void drawXORVertLine();
	void drawXORRect();
	void drawXORPixel();
	void drawPixel();
	void drawText();
	void drawBitmap();
	void drawBitmapTransparent();
	void drawFloodFill();
	void drawCopy();
	void drawDim();
	
	u16 getRandomColour();
};

#endif
