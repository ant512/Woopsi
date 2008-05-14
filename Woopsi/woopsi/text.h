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
	Text(FontBase* font, const char* text, u16 width);

	/**
	 * Destructor.
	 */
	inline ~Text() { delete [] _text; };

	/**
	 * Set the text for the object to hold.
	 * @param text Pointer to the text.
	 */
	void setText(const char* text);

	/**
	 * Add new text to the end of the current string.
	 * @param text Pointer to the text.
	 */
	void appendText(const char* text);
	
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
	 * @param lineNumber The line number to check.
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
	inline char* getLinePointer(const s32 lineNumber) const { return _text + _linePositions[lineNumber]; };

	/**
	 * Get the total height of the text in pixels.
	 * @return The total height of the text.
	 */
	inline const s32 getPixelHeight() const { return _textPixelHeight; };

	/**
	 * Get the width of the longest line in pixels.
	 * @return The width of the longest line.
	 */
	inline const u8 getPixelWidth() const { return _textPixelWidth; };

	/**
	 * Get the pixel spacing between each line of text.
	 * @return The line spacing.
	 */
	inline const u8 getLineSpacing() const { return _lineSpacing; };

	/**
	 * Get the height in pixels of a line, given as the
	 * height of the font plus the line spacing.
	 * @return The height of a line.
	 */
	inline const u8 getLineHeight() const { return _font->getHeight() + _lineSpacing; };

	/**
	 * Get the total number of lines in the text.
	 * @return The line count.
	 */
	inline const s32 getLineCount() const { return _linePositions.size() - 1; };

	/**
	 * Get a pointer to the Text object's font.
	 * @return Pointer to the font.
	 */
	FontBase* getFont();

	/**
	 * Removes lines of text from the start of the text buffer.
	 * @param lines Number of lines to remove
	 */
	void stripTopLines(const s32 lines);
	
	/**
	 * Wrap the text.
	 */
	void wrap();

private:

	FontBase* _font;						/**< Font to be used for output */
	DynamicArray<u32> _linePositions;		/**< Array containing start indexes of each wrapped line */
	u8 _lineSpacing;						/**< Spacing between lines of text */
	s32 _textPixelHeight;					/**< Total height of the wrapped text in pixels */
	u8 _textPixelWidth;						/**< Total width of the wrapped text in pixels */
	u16 _width;								/**< Width in pixels available to the text */
	char* _text;							/**< String that the object works with */
};

#endif
