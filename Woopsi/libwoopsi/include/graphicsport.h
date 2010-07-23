#ifndef _GRAPHICS_PORT_H_
#define _GRAPHICS_PORT_H_

#include <nds.h>
#include "gadget.h"
#include "woopsiarray.h"
#include "graphics.h"

namespace WoopsiUI {
	
	class FontBase;
	class FrameBuffer;
	class BitmapBase;
	
	/**
	 * GraphicsPort is the interface between a gadget and the framebuffer.  It
	 * provides drawing tools that are clipped to the visible regions of a
	 * gadget.
	 *
	 * All drawing co-ordinates should be submitted in GraphicsPort space.
	 * GraphicsPort space is relative to the x and y co-ordinates given to an
	 * instance of the GraphicsPort class via its constructor.  The width and
	 * height parameters define the size of that space.  The x and y
	 * co-ordinates submitted to the constructor should be relative to the
	 * gadget.  The getClipRect() method returns the rect in GraphicsPort
	 * space.
	 *
	 * Internally, all co-ordinates are converted to Woopsi-space for
	 * manipulation. However, when drawing all co-ordinates are converted from
	 * Woopsi-space to framebuffer-space.
	 *
	 * This class can only draw to one of the DS' framebuffers.  This means that
	 * a gadget cannot span both of the DS' screens.  If a gadget attempts to
	 * span both screens (as in the case of the Woopsi gadget), any drawing will
	 * be done to the bottom display only.  Drawing commands that should take
	 * effect on the top display will simply be clipped out.  This is why the
	 * Woopsi class creates two blank screens as backgrounds in its constructor
	 * and does not include any drawing commands.
	 *
	 * A workaround for this limitation is to manually create a new graphicsport
	 * in your gadget's draw methods and force it to use the top display's
	 * framebuffer.  You can then direct drawing commands to the upper display.
	 */
	class GraphicsPort {
	public:
		
		/**
		 * Constructor.
		 *
		 * The x, y, width and height parameters define the area within which
		 * the GraphicsPort can draw.  These should represent the co-ordinates
		 * of the gadget, or an area within the gadget, expressed in
		 * Woopsi-space co-ordinates.  A gadget's co-ordinates in Woopsi-space
		 * are given by its getX() and getY() methods.  The width and height
		 * parameters are given by the getWidth() and getHeight() methods of the
		 * gadget.
		 *
		 * To limit drawing to an area within a gadget, add the co-ordinates of
		 * the area to the values returned by getX() and getY().  For example,
		 * to create a GraphicsPort that will not draw within any of the pixels
		 * from co-ordinates (0,0) to (0,12), the x parameter should be:
		 *
		 *    getX() + 12
		 *
		 * When the co-ordinates are increased, the width must decrease.  The
		 * appropriate width for the above situation is:
		 *
		 *    getWidth() - 12
		 *
		 * In most circumstances, x/y/width/height should reflect either the
		 * dimensions and co-ordinates of the gadget or the area inside its
		 * border.
		 *
		 * @param x The x co-ordinate of the region into which the graphics port
		 * can draw.
		 * @param y The y co-ordinate of the region into which the graphics port
		 * can draw.
		 * @param width The width of the region into which the graphics port can
		 * draw.
		 * @param height The height of the region into which the graphics port
		 * can draw.
		 * @param isEnabled Set this to false to disable all drawing commands.
		 * @param bitmap The bitmap that the port will draw to. 
		 * @param clipRectList An array of clipping regions within which the
		 * class must draw.  If set, clipRect must be NULL.
		 * @param clipRect The clipping region within which the class must draw.
		 * If set, clipRectList must be NULL.
		 */
		GraphicsPort(const s16 x, const s16 y, const u16 width, const u16 height, const bool isEnabled, FrameBuffer* bitmap, const WoopsiArray<Rect>* clipRectList, const Rect* clipRect);
		
		/**
		 * Destructor.
		 */
		virtual inline ~GraphicsPort() {
			delete _graphics;
		};

		/**
		 * Sets the clip rect.  Attempts to draw outside of this region
		 * will be clipped out.  The rect's co-ordinates must be in Woopsi-space
		 * relative to the entire Woopsi system.
		 *
		 * User code should not interfere with the clipping rects inside the
		 * GraphicsPort.  Doing so will result in graphical glitches.
		 * @param clipRect The new clipping region.
		 */
		void setClipRect(const Rect& clipRect);

		/**
		 * Get the current clipping region.  The rect is automatically converted
		 * to GraphicsPort-space so that it can be retrieved in gadget drawing
		 * code in order to optmise the drawing routines.
		 * @param rect A rect that will be populated with the current
		 * clipping region.
		 */
		void getClipRect(Rect& rect) const;

		/**
		 * Return the x co-ordinate of the graphics port.
		 * @return The x co-ordinate of the graphics port.
		 */
		inline const s16 getX() const { return _rect.x; };
		
		/**
		 * Return the y co-ordinate of the graphics port.
		 * @return The y co-ordinate of the graphics port.
		 */
		const s16 getY() const { return _rect.y; };
		
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
		 * XOR the colour of the pixel at the specified co-ordinates against
		 * the supplied colour.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORPixel(s16 x, s16 y, u16 colour);
		
		/**
		 * XOR the colour of a horizontal line of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param width The width of the line.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORHorizLine(s16 x, s16 y, u16 width, u16 colour);
		
		/**
		 * XOR the colour of a vertical line of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param height The height of the line.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORVertLine(s16 x, s16 y, u16 height, u16 colour);
		
		/**
		 * XOR the colour of an unfilled rectangle of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		
		/**
		 * XOR the colour of a filled rectangle of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawFilledXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour);

		/**
		 * Invert the colour of the pixel at the specified co-ordinates.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 */
		virtual void drawXORPixel(s16 x, s16 y);
		
		/**
		 * Invert the colour of a horizontal line of pixels.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param width The width of the line.
		 */
		void drawXORHorizLine(s16 x, s16 y, u16 width);
		
		/**
		 * Invert the colour of a vertical line of pixels.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param height The height of the line.
		 */
		void drawXORVertLine(s16 x, s16 y, u16 height);
		
		/**
		 * Invert the colour of an unfilled rectangle of pixels.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		void drawXORRect(s16 x, s16 y, u16 width, u16 height);
		
		/**
		 * Invert the colour of a filled rectangle of pixels.
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
		void drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string);

		/**
		 * Draw a particular length of a string to the bitmap in a secific colour.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 * @param startIndex The start index within the string from which
		 * drawing will commence.
		 * @param length The number of characters to draw.
		 * @param colour The colour of the string.
		 */
		void drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length, u16 colour);
		
		/**
		 * Draw a portion of a string to the bitmap.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 * @param startIndex The start index within the string from which
		 * drawing will commence.
		 * @param length The number of characters to draw.
		 */
		void drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length);
		
		/**
		 * Draw a bitmap to the port's bitmap.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as
		 * the origin.
		 */
		void drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY);
		
		/**
		 * Draw a bitmap to the port's bitmap, using the supplied transparent
		 * colour as an invisible colour.  This is considerably slower than the
		 * standard bitmap drawing routine as it plots pixel-by-pixel instead of
		 * using a scanline DMA copy.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param transparentColour The transparent colour used in the bitmap.
		 */
		void drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY, u16 transparentColour);
		
		/**
		 * Draw a bitmap to the port in greyscale.  This is considerably slower
		 * than the standard bitmap drawing routine as it plots pixel-by-pixel
		 * instead of using a scanline DMA copy.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as
		 * the origin.
		 */
		virtual void drawBitmapGreyScale(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY);
		
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
		 * Copy a rectangular region from the source co-ordinates to the
		 * destination co-ordinates.  Uses the DMA for speed.  Worst-case
		 * scenario uses an off-screen buffer for situations wherein there
		 * is no vertical movement and the source and destination overlap,
		 * in which case two copies are performed for each row (source to
		 * buffer and buffer to destination).  Does not clip.
		 * @param sourceX Source x co-ord.
		 * @param sourceY Source y co-ord.
		 * @param destX Destination x co-ord.
		 * @param destY Destination y co-ord.
		 * @param width Width of the rectangle to copy.
		 * @param height Height of the rectangle to copy.
		 */
		void copy(s16 sourceX, s16 sourceY, s16 destX, s16 destY, u16 width, u16 height);
		
		/**
		 * Scroll a region by a specified distance in two dimensions.  Performs
		 * a clipped copy to achieve scroll effect.
		 * @param x X co-ord of the area to scroll.
		 * @param y Y co-ord of the area to scroll.
		 * @param xDistance Horizontal distance to scroll.
		 * @param yDistance Vertical distance to scroll.
		 * @param width Width of the area to scroll.
		 * @param height Height of the area to scroll.
		 * @param revealedRects Populated with rects representing the region
		 * uncovered by the scroll method.  This should be empty when passed,
		 * and the regions should be drawn to once the scroll has finished.
		 */
		void scroll(s16 x, s16 y, s16 xDistance, s16 yDistance, u16 width, u16 height, WoopsiArray<Rect>* revealedRects);

		/**
		 * Halve the brightness of a specified region.
		 * @param x X co-ord of the region to dim.
		 * @param y Y co-ord of the region to dim.
		 * @param width Width of the region to dim.
		 * @param height Height of the region to dim.
		 */
		void dim(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Convert the region to greyscale.
		 * @param x X co-ord of the region to change.
		 * @param y Y co-ord of the region to change.
		 * @param width Width of the region to change.
		 * @param height Height of the region to change.
		 */
		void greyScale(s16 x, s16 y, u16 width, u16 height);
		
		/**
		 * Draw an unfilled ellipse to the bitmap.
		 * @param xCentre The x co-ordinate of the ellipse's centre.
		 * @param yCentre The y co-ordinate of the ellipse's centre.
		 * @param horizRadius The size of the ellipse's horizontal radius.
		 * @param vertRadius The size of the ellipse's vertical radius.
		 * @param colour The colour of the ellipse.
		 */
		virtual void drawEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour);
		
		/**
		 * Draw a filled ellipse to the bitmap.
		 * @param xCentre The x co-ordinate of the ellipse's centre.
		 * @param yCentre The y co-ordinate of the ellipse's centre.
		 * @param horizRadius The size of the ellipse's horizontal radius.
		 * @param vertRadius The size of the ellipse's vertical radius.
		 * @param colour The colour of the ellipse.
		 */
		virtual void drawFilledEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour);

	private:
		WoopsiArray<Rect> _clipRectList;		/**< List of rects that the port must draw within. */
		Rect _rect;								/**< Total area that the port can draw within. */
		bool _isEnabled;						/**< If false, nothing will be drawn. */
		Graphics* _graphics;					/**< Used to draw to the bitmap. */
		
		void convertPortToScreenSpace(s16* x, s16* y);
		void addClipRect(const Rect& clipRect);
	};
}

#endif
