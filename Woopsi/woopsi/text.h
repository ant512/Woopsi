#ifndef _TEXT_H_
#define _TEXT_H_

#include <nds.h>
#include "fontbase.h"
#include "dynamicarray.h"
#include "woopsistring.h"

namespace WoopsiUI {

	/**
	 * This class functions as a wrapper around a char array offering
	 * more advanced functionality - it can wrap text, calculate its
	 * height in pixels, calculate the width of a row, etc.
	 */
	class Text : public WoopsiString {

	public:

		/**
		 * Constructor.
		 * @param font The font to use for this text object.
		 * @param text A string that this text object should wrap around.
		 * @param width The pixel width at which the text should wrap.
		 */
		Text(FontBase* font, const char* text, u16 width);

		/**
		 * Set the text in the string.
		 * @param text Char array to use as the new data for this string.
		 */
		virtual void setText(const char* text);

		/**
		 * Set the text in the string.
		 * @param text Character to to use as the new data for this string.
		 */
		virtual void setText(const char text);
		
		/**
		 * Append text to the end of the string.
		 * @param text String to append.
		 */
		virtual void append(const char* text);

		/**
		 * Append text to the end of the string.
		 * @param text Char to append.
		 */
		virtual void append(const char text);

		/**
		 * Insert text at the specified character index.
		 * @param text The text to insert.
		 * @param index The char index to insert at.
		 */
		virtual void insert(const char* text, const u32 index);

		/**
		 * Insert text at the specified character index.
		 * @param text Char to insert.
		 * @param index The char index to insert at.
		 */
		virtual void insert(const char text, const u32 index);

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex The char index to start removing from.
		 */
		virtual void remove(const u32 startIndex);

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex The char index to start removing from.
		 * @param count The number of chars to remove.
		 */
		virtual void remove(const u32 startIndex, const u32 count);
		
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
		inline const char* getLinePointer(const s32 lineNumber) const { return getCharArray() + _linePositions[lineNumber]; };

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
		 * Wrap all of the text.
		 */
		void wrap();

		/**
		 * Wrap the text from the line containing the specified char index onwards.
		 * @param charIndex The index of the char to start wrapping from; note
		 * that the wrapping function will re-wrap that entire line of text.
		 */
		void wrap(u32 charIndex);

		/**
		 * Get the index of the line of text that contains the specified index
		 * within the raw char array.
		 * @param index The index to locate within the wrapped lines of text.
		 * @return The number of the line of wrapped text that contains the
		 * specified index.
		 */
		u32 getLineContainingCharIndex(u32 index);

		/**
		 * Gets the index within the char array that represents the start of the line of
		 * text indicated by the line parameter.
		 * @param line The line number to locate within the char array.
		 * @return The index within the char array of the start of the supplied line.
		 */
		u32 getLineStartIndex(u32 line) { return _linePositions[line]; };

	private:
		/**
		 * Struct defining the position and length of a longest line within the _linePositions
		 * array.
		 */
		typedef struct {
			u32 index;
			u8 width;
		} LongestLine;
		
		FontBase* _font;							/**< Font to be used for output */
		DynamicArray<u32> _linePositions;			/**< Array containing start indexes of each wrapped line */
		DynamicArray<LongestLine> _longestLines;	/**< Array containing data describing successively longer wrapped lines */
		u8 _lineSpacing;							/**< Spacing between lines of text */
		s32 _textPixelHeight;						/**<Total height of the wrapped text in pixels */
		u8 _textPixelWidth;							/**< Total width of the wrapped text in pixels */
		u16 _width;									/**< Width in pixels available to the text */
	};
}

#endif
