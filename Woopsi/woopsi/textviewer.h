#ifndef _TEXTVIEWER_H_
#define _TEXTVIEWER_H_

#include <nds.h>
#include <vector>
#include "textwriter.h"
#include "gadget.h"
#include "fontbase.h"
#include "text.h"
#include "graphicsport.h"

using namespace std;

class TextViewer : public Gadget {

public:
	TextViewer(s16 x, s16 y, u16 width, u16 height, s32 textY, char* text, FontBase* font = NULL);
	virtual ~TextViewer();
	virtual void draw();
	virtual void draw(Rect clipRect);
	virtual void setTextY(s32 textY);
	virtual void setText(char* text);
	virtual void setFont(FontBase* font);
	virtual Text* getText();
	virtual u32 getTextY();
	
	virtual void scrollUp(s32 incY);
	virtual void scrollDown(s32 incY);

	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);
	virtual bool resize(u16 width, u16 height);

protected:

	s32 _firstLineIndex;
	s32 _lastLineIndex;
	s32 _textY;
	u16 _innerWidth;
	u16 _innerHeight;
	s16 _innerX;
	s16 _innerY;
	Text* _text;

	void increaseTextY(s32 incY);
	void drawLastLines(s32 incY);
	void drawFirstLines(s32 incY);
	void drawScreen();
	void drawScreen(Rect clipRect);
	void wrapText();
};

#endif
