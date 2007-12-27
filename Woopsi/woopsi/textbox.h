#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <nds.h>
#include "textwriter.h"
#include "gadget.h"
#include "fontbase.h"
#include "graphicsport.h"

using namespace std;

class Textbox : public Gadget {
public:
	enum TextPositionHoriz {
		TEXT_POSITION_HORIZ_CENTRE = 0,
		TEXT_POSITION_HORIZ_LEFT = 1,
		TEXT_POSITION_HORIZ_RIGHT = 2
	};

	enum TextPositionVert {
		TEXT_POSITION_VERT_CENTRE = 0,
		TEXT_POSITION_VERT_TOP = 1,
		TEXT_POSITION_VERT_BOTTOM = 2
	};

	Textbox(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font = NULL);
	Textbox(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font = NULL);
	virtual ~Textbox();

	virtual void draw(Rect clipRect);
	virtual void draw();

	virtual void setTextPositionHoriz(TextPositionHoriz position);
	virtual void setTextPositionVert(TextPositionVert position);
	
	virtual char* getText();
	virtual void setText(const char* text);
	virtual void addText(const char* text);

	virtual bool click(s16 x, s16 y);
	virtual bool release(s16 x, s16 y);


protected:
	char* _text;
	u16 _textX;
	u16 _textY;
	u8 _padding;
	TextPositionHoriz _hPos;
	TextPositionVert _vPos;

	void calculateTextPosition();

};

#endif
