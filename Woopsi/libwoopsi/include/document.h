#ifndef _TEXT_H_
#define _TEXT_H_

#include <nds.h>
#include "fontbase.h"
#include "woopsiarray.h"
#include "woopsistring.h"

namespace WoopsiUI {

	/**
	 * Contains a WoopsiString and can wrap it to fit within a given width for a
	 * given font.
	 */
	class Document {

	public:

		/**
		 * Constructor.
		 * @param font The font to use for this object.
		 * @param text A string that this object should wrap around.
		 * @param width The pixel width at which the text should wrap.
		 */
		Document(FontBase* font, const WoopsiString& text, u16 width);

		/**
		 * Set the text in the string.
		 * @param text The new string.
		 */
		virtual void setText(const WoopsiString& text);

		/**
		 * Set the text in the string.
		 * @param text Char array to use as the new data for this string.
		 */
		virtual void setText(const char* text);

		/**
		 * Set the text in the string.
		 * @param text Character to to use as the new data for this string.
		 */
		virtual void setText(const u32 text);
		
		/**
		 * Append text to the end of the string.
		 * @param text String to append.
		 */
		virtual void append(const WoopsiString& text);

		/**
		 * Insert text at the specified character index.
		 * @param text The text to insert.
		 * @param index The char index to insert at.
		 */
		virtual void insert(const WoopsiString& text, const s32 index);

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex The char index to start removing from.
		 */
		virtual void remove(const s32 startIndex);

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex The char index to start removing from.
		 * @param count The number of chars to remove.
		 */
		virtual void remove(const s32 startIndex, const s32 count);
		
		/**
		 * Set the vertical spacing between rows of text.
		 * @param lineSpacing The line spacing.
		 */
		void setLineSpacing(u8 lineSpacing);

		/**
		 * Sets the pixel width of the text; text wider than this will
		 * automatically wrap.
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
		const u8 getLineLength(const s32 lineNumber) const;

		/**
		 * Get the number of characters in the specified line number, ignoring
		 * any trailing blank characters.
		 * @param lineNumber The line number to check.
		 * @return The number of characters in the line.
		 */
		const s16 getLineTrimmedLength(const s32 lineNumber) const;

		/**
		 * Get the width in pixels of the specified line number.
		 * @param lineNumber The line number to check.
		 * @return The pixel width of the line.
		 */
		const s16 getLinePixelLength(const s32 lineNumber) const;

		/**
		 * Get the width in pixels of the specified line number, ignoring any
		 * trailing blank characters.
		 * @param lineNumber The line number to check.
		 * @return The pixel width of the line.
		 */
		const s16 getLineTrimmedPixelLength(const s32 lineNumber) const;

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
		 * Get a pointer to the Document object's font.
		 * @return Pointer to the font.
		 */
		FontBase* getFont() const;

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
		 * Wrap the text from the line containing the specified char index
		 * onwards.
		 * @param charIndex The index of the char to start wrapping from; note
		 * that the wrapping function will re-wrap that entire line of text.
		 */
		void wrap(s32 charIndex);

		/**
		 * Get the index of the line of text that contains the specified index
		 * within the raw char array.
		 * @param index The index to locate within the wrapped lines of text.
		 * @return The number of the line of wrapped text that contains the
		 * specified index.
		 */
		const s32 getLineContainingCharIndex(const s32 index) const;

		/**
		 * Gets the index within the char array that represents the start of the
		 * line of text indicated by the line parameter.
		 * @param line The line number to locate within the char array.
		 * @return The index within the char array of the start of the supplied
		 * line.
		 */
		const s32 getLineStartIndex(const s32 line) const { return _linePositions[line]; };

		/**
		 * Get a reference to the internal string.  String is constant to
		 * prevent it being changed without notifying the document.  Any change
		 * to a string should cause a re-wrap operation.
		 * @return The internal string.
		 */
		const WoopsiString& getText() const { return _text; };

	private:
		/**
		 * Struct defining the position and length of a longest line within the
		 * _linePositions array.
		 */
		typedef struct {
			s32 index;
			u8 width;
		} LongestLine;
		
		FontBase* _font;							/**< Font to be used for output */
		WoopsiArray<s32> _linePositions;			/**< Array containing start indexes of each wrapped line */
		WoopsiArray<LongestLine> _longestLines;		/**< Array containing data describing successively longer wrapped lines */
		u8 _lineSpacing;							/**< Spacing between lines of text */
		s32 _textPixelHeight;						/**< Total height of the wrapped text in pixels */
		u8 _textPixelWidth;							/**< Total width of the wrapped text in pixels */
		u16 _width;									/**< Width in pixels available to the text */
		WoopsiString _text;							/**< Content of the document. */
	};
}

#endif
