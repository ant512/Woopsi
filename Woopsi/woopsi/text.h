#ifndef _TEXT_H_
#define _TEXT_H_

#include <nds.h>
#include "fontbase.h"
#include "dynamicarray.h"

using namespace std;

/**
 * This class functions as a wrapper around a char array offering
 * more advanced functionality - it can wrap text, calculate its
 * height in pixels, calculate the width of a row, etc.
 */
class Text {

public:

	/**
	 * Constructor.
	 * @param font The font to use for this text object.
	 * @param text A string that this text object should wrap around.
	 * @param width The pixel width at which the text should wrap.
	 */
	Text(FontBase* font, char* text, u16 width);

	/**
	 * Destructor.
	 */
	~Text();

	/**
	 * Set the text for the object to hold.
	 * @param text Pointer to the text.
	 */
	void setText(char* text);
	
	/**
	 * Set the vertical spacing between rows of text.
	 * @param lineSpacing The line spacing.
	 */
	void setLineSpacing(u8 lineSpacing);

	/**
	 * Sets the pixel width of the text; text wider than 
	 * this will automatically wrap.
	 * @param width Maximum pixel width of the text.
	 */
	void setWidth(u16 width);

	/**
	 * Set the font to use.
	 * @param font Pointer to the new font.
	 */
	void setFont(FontBase* font);

	/**
	 * Get the number of characters in the specified line number.
	 * @param lineNumber The line number to check.
	 * @return The number of characters in the line.
	 */
	u8 getLineLength(s32 lineNumber);

	/**
	 * Get the number of characters in the specified line number,
	 * ignoring any trailing blank characters.
	 * @param The line number to check.
	 * @return The number of characters in the line.
	 */
	u8 getLineTrimmedLength(s32 lineNumber);

	/**
	 * Get the width in pixels of the specified line number.
	 * @param lineNumber The line number to check.
	 * @return The pixel width of the line.
	 */
	u8 getLinePixelLength(s32 lineNumber);

	/**
	 * Get the width in pixels of the specified line number,
	 * ignoring any trailing blank characters.
	 * @param lineNumber The line number to check.
	 * @return The pixel width of the line.
	 */
	u8 getLineTrimmedPixelLength(s32 lineNumber);

	/**
	 * Get a pointer to the specified line.
	 * @param lineNumber The line to get a pointer to.
	 * @return A pointer to the line.
	 */
	char* getLinePointer(s32 lineNumber);

	/**
	 * Get the total height of the text in pixels.
	 * @return The total height of the text.
	 */
	const s32 getPixelHeight() const;

	/**
	 * Get the width of the longest line in pixels.
	 * @return The width of the longest line.
	 */
	const u8 getPixelWidth() const;

	/**
	 * Get the pixel spacing between each line of text.
	 * @return The line spacing.
	 */
	const u8 getLineSpacing() const;

	/**
	 * Get the height in pixels of a line, given as the
	 * height of the font plus the line spacing.
	 * @return The height of a line.
	 */
	const u8 getLineHeight() const;

	/**
	 * Get the total number of lines in the text.
	 * @return The line count.
	 */
	const s32 getLineCount() const;

	/**
	 * Get a pointer to the Text object's font.
	 * @return Pointer to the font.
	 */
	FontBase* getFont();
	
private:

	FontBase* _font;
	DynamicArray<u32> _linePositions;
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
