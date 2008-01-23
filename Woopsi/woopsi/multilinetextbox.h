#ifndef _MULTILINE_TEXTBOX_H_
#define _MULTILINE_TEXTBOX_H_

#include <nds.h>
#include "textwriter.h"
#include "gadget.h"
#include "scrollingpanel.h"
#include "fontbase.h"
#include "text.h"
#include "graphicsport.h"

using namespace std;

class MultiLineTextBox : public ScrollingPanel {
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

	MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, char* text, u32 flags, s16 maxRows = 0, FontBase* font = NULL);
	virtual ~MultiLineTextBox();

	virtual void draw(Rect clipRect);
	virtual void draw();

	virtual void setTextPositionHoriz(TextPositionHoriz position);
	virtual void setTextPositionVert(TextPositionVert position);
	virtual void setAutomaticDrawing(bool autoDrawing);
	
	virtual const u16 getPageCount() const;
	virtual const u16 getCurrentPage() const;
	
	virtual char* getRawText();
	virtual const Text* getText() const;
	virtual void setText(char* text);
	virtual void addText(char* text);

	virtual void setFont(FontBase* font);

protected:
	char* _rawText;
	Text* _text;
	u8 _visibleRows;
	s16 _maxRows;
	u32 _topRow;
	u8 _padding;
	TextPositionHoriz _hPos;
	TextPositionVert _vPos;
	bool _autoDrawing;

	u8 getRowX(u8 rowPixelWidth);
	s16 getRowY(u8 screenRow, u8 screenRows);
	void calculateTotalVisibleRows();
	void stripTopLines(const u32 lines);
	
	virtual void setText(char* text, bool raiseEvent);
};

#endif
