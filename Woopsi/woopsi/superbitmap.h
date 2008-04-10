#ifndef _SUPERBITMAP_H_
#define _SUPERBITMAP_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

class TextWriter;

/**
 * The SuperBitmap class provides a set of 2D drawing tools and a bitmap in RAM to draw on.
 * The class manifests itself as a gadget that can be scrolled around using the stylus.
 */
class SuperBitmap : public Gadget {

public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the gadget.
	 * @param y The y co-ordinate of the gadget.
	 * @param width The width of the gadget.
	 * @param height The height of the gadget.
	 * @param bitmapWidth The desired width of the gadget's bitmap.
	 * @param bitmapHeight The desired height of the gadget's bitmap.
	 * @param isDecoration Specify if the gadget is a decoration or not.  If set to true
	 * the gadget will function as a background image.
	 * @param font The font to use for text output.
	 */
	SuperBitmap(s16 x, s16 y, u16 width, u16 height, u16 bitmapWidth, u16 bitmapHeight, bool isDecoration, FontBase* font = NULL);
	
	/**
	 * Get the colour of the pixel at the specified co-ordinates
	 * @param x The x co-ordinate of the pixel.
	 * @param y The y co-ordinate of the pixel.
	 * @return The colour of the pixel.
	 */
	const u16 getPixel(s16 x, s16 y) const;

	/**
	 * Get a pointer to the internal bitmap.
	 * @return Pointer to the internal bitmap.
	 */
	inline const u16* getBitmap() const { return _bitmap; };

	/**
	 * Draws the gadget to the frawebuffer.
	 */
	virtual inline void draw() { Gadget::draw(); };
	
	/**
	 * Draws the region of the gadget that falls within the clipping region.  Should not
	 * be called.
	 * @param clipRect The rectangle to clip to.
	 */
	virtual void draw(Rect clipRect);
	
	/**
	 * Draw a pixel to the internal bitmap.
	 * @param x The x co-ordinate of the pixel.
	 * @param y The y co-ordinate of the pixel.
	 * @param colour The colour of the pixel.
	 */
	virtual void drawPixel(s16 x, s16 y, u16 colour);
	
	/**
	 * Draw a filled rectangle to the internal bitmap.
	 * @param x The x co-ordinate of the rectangle.
	 * @param y The y co-ordinate of the rectangle.
	 * @param width The width of the rectangle.
	 * @param height The height of the rectangle.
	 * @param colour The colour of the rectangle.
	 */
	virtual void drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
	
	/**
	 * Draw a horizontal line to the internal bitmap.
	 * @param x The x co-ordinate of the line.
	 * @param y The y co-ordinate of the line.
	 * @param width The width of the line.
	 * @param colour The colour of the line.
	 */
	virtual void drawHorizLine(s16 x, s16 y, u16 width, u16 colour);
	
	/**
	 * Draw a vertical line to the internal bitmap.
	 * @param x The x co-ordinate of the line.
	 * @param y The y co-ordinate of the line.
	 * @param height The height of the line.
	 * @param colour The colour of the line.
	 */
	virtual void drawVertLine(s16 x, s16 y, u16 height, u16 colour);
	
	/**
	 * Draw an unfilled rectangle to the internal bitmap.
	 * @param x The x co-ordinate of the rectangle.
	 * @param y The y co-ordinate of the rectangle.
	 * @param width The width of the rectangle.
	 * @param height The height of the rectangle.
	 * @param colour The colour of the rectangle.
	 */
	virtual void drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
	
	/**
	 * Draw a line to the internal bitmap.
	 * @param x1 The x co-ordinate of the start point of the line.
	 * @param y1 The y co-ordinate of the start point of the line.
	 * @param x2 The x co-ordinate of the end point of the line.
	 * @param y2 The y co-ordinate of the end point of the line.
	 * @param colour The colour of the rectangle.
	 */
	virtual void drawLine(s16 x1, s16 y1, u16 x2, u16 y2, u16 colour);
	
	/**
	 * Draw an unfilled circle to the internal bitmap.
	 * @param x0 The x co-ordinate of the circle.
	 * @param y0 The y co-ordinate of the circle.
	 * @param radius The radius of the circle.
	 * @param colour The colour of the circle.
	 */
	virtual void drawCircle(s16 x0, s16 y0, u16 radius, u16 colour);
	
	/**
	 * Draw a filled circle to the internal bitmap.
	 * @param x0 The x co-ordinate of the circle.
	 * @param y0 The y co-ordinate of the circle.
	 * @param radius The radius of the circle.
	 * @param colour The colour of the circle.
	 */
	virtual void drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour);
	
	/**
	 * Draw a string to the internal bitmap.
	 * @param x The x co-ordinate of the string.
	 * @param y The y co-ordinate of the string.
	 * @param font The font to draw with.
	 * @param string The string to output.
	 */
	virtual void drawText(s16 x, s16 y, FontBase* font, char* string);
	
	/**
	 * Draw a string to the internal bitmap in a specific colour.
	 * @param x The x co-ordinate of the string.
	 * @param y The y co-ordinate of the string.
	 * @param font The font to draw with.
	 * @param string The string to output.
	 * @param colour The colour of the string.
	 */
	virtual void drawText(s16 x, s16 y, FontBase* font, char* string, u16 colour);
	
	/**
	 * Draw an external bitmap to the internal bitmap.
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
	virtual void drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight);
	
	/**
	 * Fill a region of the internal bitmap with the specified colour.
	 * @param x The x co-ordinate to use as the starting point of the fill.
	 * @param y The y co-ordinate to use as the starting point of the fill.
	 * @param newColour The colour to fill with.
	 */
	virtual void floodFill(s16 x, s16 y, u16 newColour);

	/**
	 * Erases the internal bitmap by filling it with the gadget's back colour.
	 */
	virtual void clearBitmap();

	/**
	 * Drag the gadget.
	 * @param x The x co-ordinate of the drag.
	 * @param y The y co-ordinate of the drag.
	 * @param vX The horizontal drag distance.
	 * @param vY The vertical drag distance.
	 */
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

protected:
	s32 _bitmapX;									/**< X co-ordinate of the bitmap relative to the gadget */
	s32 _bitmapY;									/**< Y co-ordinate of the bitmap relative to the gadget */
	u16 _bitmapWidth;								/**< Width of the bitmap */
	u16 _bitmapHeight;								/**< Height of the bitmap */
	u16* _bitmap __attribute__ ((aligned (4)));		/**< Bitmap */
	
	/**
	 * Allocate the bitmap memory and clear it.
	 */
	virtual void initBitmap();

	/**
	 * Remove the last item from the flood fill stack and insert its properties into the
	 * supplied x and y variables.
	 * @param x The x co-ordinate of the pixel from the stack (output by the function).
	 * @param y The y co-ordinate of the pixel from the stack (output by the function).
	 * @param stack Pointer to the stack to pop from.
	 */
	bool popStack(s16* x, s16* y, DynamicArray<u16>* stack);
	
	/**
	 * Push new co-ordinates onto the stack.
	 * @param x The x co-ordinate of the pixel to add to the stack.
	 * @param y The y co-ordinate of the pixel to add to the stack.
	 * @param stack The stack to add a new value to.
	 */
	void pushStack(s16 x, s16 y, DynamicArray<u16>* stack) ;
	
	/**
	 * Clip the supplied rectangular dimensions to the size of the internal bitmap.
	 * @param x The x co-ordinate of the rectangle to clip (modified by the function).
	 * @param y The y co-ordinate of the rectangle to clip (modified by the function).
	 * @param width The width of the rectangle to clip (modified by the function).
	 * @param height The height of the rectangle to clip (modified by the function).
	 */
	bool clipBitmapCoordinates(s16* x, s16* y, u16* width, u16* height);

	/**
	 * Destructor.
	 */
	virtual inline ~SuperBitmap() {
		delete[] _bitmap;
	};
};

#endif
