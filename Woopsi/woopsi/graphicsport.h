#ifndef _GRAPHICS_PORT_H_
#define _GRAPHICS_PORT_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

	class FontBase;

	/**
	 * GraphicsPort is the interface between a gadget and the framebuffer.  It provides
	 * drawing tools that are clipped to the visible regions of a gadget.  This class can only
	 * draw to one of the DS' framebuffers.  It makes extensive use of the DMA hardware and does
	 * not call DC_FlushRange(), which will cause problems if it attempts to draw to anything
	 * other than VRAM.
	 */
	class GraphicsPort {
	public:
		
		/**
		 * Constructor.
		 * @param gadget Pointer to the gadget that this port will draw into.
		 * @param x The x co-ordinate of the graphics port.
		 * @param y The y co-ordinate of the graphics port.
		 * @param width The width of the graphics port.
		 * @param height The height of the graphics port.
		 * @param bitmap The bitmap that the port will draw to. 
		 * @param bitmapWidth The width of the bitmap being drawn to.
		 * @param bitmapHeight The height of the bitmap being drawn to.
		 * @param clipRect The clipping region within which the class must draw.
		 */
		GraphicsPort(Gadget* const gadget, const s16 x, const s16 y, const u16 width, const u16 height, u16* const bitmap, const u16 bitmapWidth, const u16 bitmapHeight, const Gadget::Rect* clipRect = NULL);
		
		/**
		 * Destructor.
		 */
		inline ~GraphicsPort() {
			delete _clipRect;
		};

		/**
		 * Return the x co-ordinate of the graphics port.
		 * @return The x co-ordinate of the graphics port.
		 */
		inline const s16 getX() const { return _rect.x + _gadget->getX(); };
		
		/**
		 * Return the y co-ordinate of the graphics port.
		 * @return The y co-ordinate of the graphics port.
		 */
		const s16 getY() const { return _rect.y + _gadget->getY(); };

		/**
		 * Draw a pixel to the bitmap.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @param colour The colour of the pixel.
		 */
		void drawPixel(s16 x, s16 y, u16 colour);
		
		/**
		 * Draw a filled rectangle to the bitmap.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour of the rectangle.
		 */
		void drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		
		/**
		 * Draw a horizontal line to the bitmap
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param width The width of the line.
		 * @param colour The colour of the line.
		 */
		void drawHorizLine(s16 x, s16 y, s16 width, u16 colour);
		
		/**
		 * Draw a vertical line to the bitmap
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param height The height of the line.
		 * @param colour The colour of the line.
		 */
		void drawVertLine(s16 x, s16 y, s16 height, u16 colour);
		
		/**
		 * Draw an unfilled rectangle to the bitmap
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour of the rectangle.
		 */
		void drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		
		/**
		 * Draw a bevelled rectangle to the bitmap.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param shineColour The colour of the top/left sides.
		 * @param shadowColour The colour of the bottom/right sides.
		 */
		void drawBevelledRect(s16 x, s16 y, u16 width, u16 height, u16 shineColour, u16 shadowColour);
		
		/**
		 * Draw a bevelled rectangle to the bitmap using the gadget's border details as the
		 * basis of the bevel colours.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		void drawBevelledRect(s16 x, s16 y, u16 width, u16 height);
		
		/**
		 * Draw a filled circle to the bitmap.
		 * @param x0 The x co-ordinate of the circle.
		 * @param y0 The y co-ordinate of the circle.
		 * @param radius The radius of the circle.
		 * @param colour The colour of the circle.
		 */
		void drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour);
		
		/**
		 * Invert the colour of the pixel at the specified co-ordinates.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 */
		void drawXORPixel(s16 x, s16 y);
		
		/**
		 * Invert the colour of a horizontal line of pixels.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param width The width of the line.
		 */
		void drawXORHorizLine(s16 x, s16 y, s16 width);
		
		/**
		 * Invert the colour of a vertical line of pixels.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param height The height of the line.
		 */
		void drawXORVertLine(s16 x, s16 y, s16 height);
		
		/**
		 * Invert the colour of an unfilled rectangle of pixels.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		void drawXORRect(s16 x, s16 y, u16 width, u16 height);
		
		/**
		 * Invert the colour of an filled rectangle of pixels.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		void drawFilledXORRect(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Draw a string to the bitmap.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 */
		void drawText(s16 x, s16 y, FontBase* font, const char* string);
		
		/**
		 * Draw a particular length of a string to the bitmap.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param length The number of characters to output.
		 * @param string The string to output.
		 */
		void drawText(s16 x, s16 y, FontBase* font, u16 length, const char* string);
		
		/**
		 * Draw a single character to the bitmap.
		 * @param x The x co-ordinate of the character.
		 * @param y The y co-ordinate of the character.
		 * @param font The font to draw with.
		 * @param letter The character to output.
		 */
		void drawText(s16 x, s16 y, FontBase* font, char letter);
		
		/**
		 * Draw a string to the bitmap in a specific colour.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 * @param colour The colour of the string.
		 */
		void drawText(s16 x, s16 y, FontBase* font, const char* string, u16 colour);
		
		/**
		 * Draw a single character to the bitmap in a specific colour.
		 * @param x The x co-ordinate of the character.
		 * @param y The y co-ordinate of the character.
		 * @param font The font to draw with.
		 * @param letter The character to output.
		 * @param colour The colour of the character.
		 */
		void drawText(s16 x, s16 y, FontBase* font, char letter, u16 colour);
		
		/**
		 * Draw a bitmap to the port's bitmap.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapWidth The width of the supplied bitmap.
		 * @param bitmapHeight The height of the supplied bitmap.
		 */
		void drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight);
		
		/**
		 * Draw a bitmap to the port's bitmap, using the supplied transparent colour
		 * as an invisible colour.  This is considerably slower than the standard bitmap
		 * drawing routine as it plots pixel-by-pixel instead of using a scanline DMA copy.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapWidth The width of the supplied bitmap.
		 * @param bitmapHeight The height of the supplied bitmap.
		 * @param transparentColour The transparent colour used in the bitmap.
		 */
		void drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight, u16 transparentColour);

		/**
		 * Draw a line to the port's bitmap.
		 * @param x1 The x co-ordinate of the start point of the line.
		 * @param y1 The y co-ordinate of the start point of the line.
		 * @param x2 The x co-ordinate of the end point of the line.
		 * @param y2 The y co-ordinate of the end point of the line.
		 * @param colour The colour of the line.
		 */
		void drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour);

		/**
		 * Erases the graphics port's output by redrawing its gadget.
		 */
		void clear();

	private:
		Gadget* _gadget;							/**< Pointer to the gadget that the port will draw to */
		Gadget::Rect* _clipRect;					/**< Clipping rect that the port must draw within */
		Gadget::Rect _rect;							/**< Total area that the port can draw within */
		u16* _bitmap;								/**< Bitmap that the port will draw to (should be in VRAM) */
		u16 _bitmapWidth;							/**< Width of the bitmap that the port will draw to */
		u16 _bitmapHeight;							/**< Height of the bitmap that the port will draw to */

		// Internal clipping routines
		void clipPixel(s16 x, s16 y, u16 colour, const Gadget::Rect& clipRect);
		void clipFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour, const Gadget::Rect& clipRect);
		void clipHorizLine(s16 x, s16 y, s16 width, u16 colour, const Gadget::Rect& clipRect);
		void clipVertLine(s16 x, s16 y, s16 height, u16 colour, const Gadget::Rect& clipRect);
		void clipBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight, const Gadget::Rect& clipRect);
		void clipBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight, u16 transparentColour, const Gadget::Rect& clipRect);
		void clipText(s16 x, s16 y, FontBase* font, u16 length, const char* string, const Gadget::Rect& clipRect);
		void clipFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour, const Gadget::Rect& clipRect);
		void clipXORPixel(s16 x, s16 y, const Gadget::Rect& clipRect);
		void clipXORHorizLine(s16 x, s16 y, s16 width, const Gadget::Rect& clipRect);
		void clipXORVertLine(s16 x, s16 y, s16 height, const Gadget::Rect& clipRect);
		void clipLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour, const Gadget::Rect& clipRect);

		// Drawing functions that take pre-clipped values
		void drawClippedPixel(s16 x, s16 y, u16 colour);
		void drawClippedFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		void drawClippedHorizLine(s16 x, s16 y, s16 width, u16 colour);
		void drawClippedVertLine(s16 x, s16 y, s16 height, u16 colour);
		void drawClippedBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight);
		void drawClippedBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight, u16 transparentColour);
		void drawClippedXORPixel(s16 x, s16 y);
		void drawClippedXORHorizLine(s16 x, s16 y, u16 width);
		void drawClippedXORVertLine(s16 x, s16 y, u16 height);
		void drawClippedXORRect(s16 x, s16 y, u16 width, u16 height);
		void drawClippedLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour);

		void convertPortToScreenSpace(s16* x, s16* y);
		bool clipCoordinates(s16* x1, s16* y1, s16* x2, s16* y2, const Gadget::Rect& clipRect);
		u8 getClipLineOutCode(s16 x, s16 y, s16 xMin, s16 yMin, s16 xMax, s16 yMax);
	};
}

#endif
