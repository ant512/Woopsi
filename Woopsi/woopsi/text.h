#ifndef _TEXT_H_
#define _TEXT_H_

#include <nds.h>
#include <vector>
#include "fontbase.h"
#include "debug.h"

using namespace std;

class Text {

public:
	Text(FontBase* font, char* text, u16 width);
	~Text();
	void setText(char* text);
	void setLineSpacing(u8 lineSpacing);
	void setWidth(u16 width);
	void setFont(FontBase* font);
	u8 getLineLength(s32 lineNumber);
	u8 getLineTrimmedLength(s32 lineNumber);
	u8 getLinePixelLength(s32 lineNumber);
	u8 getLineTrimmedPixelLength(s32 lineNumber);
	char* getLinePointer(s32 lineNumber);
	s32 getPixelHeight();
	u8 getPixelWidth();
	u8 getLineSpacing();
	u8 getLineHeight();
	s32 getLineCount();
	FontBase* getFont();
	
private:

	FontBase* _font;
	vector<u32> _linePositions;
	s32 _totalLines;
	u8 _lineSpacing;
	u8 _lineHeight;
	s32 _textPixelHeight;
	u8 _textPixelWidth;
	u8 _maxLineLength;
	s32 _firstLineIndex;
	s32 _lastLineIndex;
	s32 _textY;
	u16 _width;
	char* _text;

	void calculateLineHeight();
	void calculateMaxLineLength();
	void calculateTextPixelHeight();
	void wrap();
};

#endif
