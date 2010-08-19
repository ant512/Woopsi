#ifndef _RECT_H_
#define _RECT_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Class describing a rectangle.
	 */
	class Rect {
	public:
		s16 x;							/**< X co-ordinate of the rectangle. */
		s16 y;							/**< Y co-ordinate of the rectangle. */
		s32 width;						/**< Width of the rectangle. */
		s32 height;						/**< Height of the rectangle. */

		/**
		 * Constructor.
		 */
		Rect();

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the rect.
		 * @param y The y co-ordinate of the rect.
		 * @param width The width of the rect.
		 * @param height The height of the rect.
		 */
		Rect(s16 x, s16 y, s32 width, s32 height);

		/**
		 * Copy constructor.
		 * @param rect Rect to copy.
		 */
		Rect(const Rect& rect);

		/**
		 * Create a rect object from the supplied co-ordinates.
		 * @param x1 The x co-ordinate of the rect's top-left corner.
		 * @param y1 The y co-ordinate of the rect's top-left corner.
		 * @param x2 The x co-ordinate of the rect's bottom-right corner.
		 * @param y2 The y co-ordinate of the rect's bottom-right corner.
		 * @return A new rect.
		 */
		static Rect fromCoordinates(s16 x1, s16 y1, s16 x2, s16 y2);

		/**
		 * Get the rect's x co-ordinate.
		 * @return The rect's x co-ordinate.
		 */
		inline s16 getX() const { return x; };

		/**
		 * Get the rect's y co-ordinate.
		 * @return The rect's y co-ordinate.
		 */
		inline s16 getY() const { return y; };

		/**
		 * Get the rect's width.
		 * @return The rect's width.
		 */
		inline s32 getWidth() const { return width; };

		/**
		 * Get the rect's height.
		 * @return The rect's height.
		 */
		inline s32 getHeight() const { return height; };

		/**
		 * Set the rect's x co-ordinate.
		 * @param x The new x co-ordinate.
		 */
		inline void setX(s16 x) { this->x = x; };

		/**
		 * Set the rect's y co-ordinate.
		 * @param y The new y co-ordinate.
		 */
		inline void setY(s16 y) { this->y = y; };

		/**
		 * Set the rect's width.
		 * @param width The new width.
		 */
		inline void setWidth(s32 width) { this->width = width; };

		/**
		 * Set the rect's height.
		 * @param height The new height.
		 */
		inline void setHeight(s32 height) { this->height = height; };

		/**
		 * Set the x co-ordinate of the rect's bottom-right corner.  If x2 is less
		 * than the rect's current x co-ordinate the method automatically adjusts
		 * the co-ords so that the rect's width is never negative.  Changing this
		 * property will change the width of the rect.
		 * @param x2 The x co-ordinate of the rect's bottom-right corner.
		 */
		void setX2(s16 x2);

		/**
		 * Set the y co-ordinate of the rect's bottom-right corner.  If y2 is less
		 * than the rect's current y co-ordinate the method automatically adjusts
		 * the co-ords so that the rect's height is never negative.  Changing this
		 * property will change the height of the rect.
		 * @param y2 The y co-ordinate of the rect's bottom-right corner.
		 */
		void setY2(s16 y2);

		/**
		 * Get the x co-ordinate of the rect's bottom-right corner.
		 * @return The x co-ordinate of the rect's bottom-right corner.
		 */
		inline s16 getX2() const { return x + (width - 1); };

		/**
		 * Get the y co-ordinate of the rect's bottom-right corner.
		 * @return The y co-ordinate of the rect's bottom-right corner.
		 */
		inline s16 getY2() const { return y + (height - 1); };

		/**
		 * Determines if the rectangle has two dimensions; in other words, does it
		 * have both height and width?  Negative width or height is considered not to
		 * be valid.
		 * @return True if the rect has height and width; false if not.
		 */
		bool hasDimensions() const;

		/**
		 * Populates dest with a rectangle representating the intersection
		 * of this rectangle and rect.
		 * @param rect The rectangle to intersect with this.
		 * @param dest The destination rectangle.
		 */
		void getIntersect(const Rect& rect, Rect& dest) const;

		/**
		 * Populates dest with a rectangle representating the smallest
		 * rectangle that contains this rectangle and rect.
		 * @param rect The rectangle to add to this.
		 * @param dest The destination rectangle.
		 */
		void getAddition(const Rect& rect, Rect& dest) const;

		/**
		 * Clips this rect to the region that intersects the supplied rect.
		 */
		void clipToIntersect(const Rect& rect);

		/**
		 * Expands this rect so that it includes the area described by the supplied
		 * rect.
		 */
		void expandToInclude(const Rect& rect);

		/**
		 * Check if the supplied rect intersects this.
		 * @param rect Rect to check for intersection with this.
		 * @return True if the rect intersects this; false if not.
		 */
		bool intersects(const Rect& rect) const;

		/**
		 * Check if the rect contains the supplied point.
		 * @param x X co-ord of the point.
		 * @param y Y co-ord of the point.
		 * @return True if the rect contains the point; false if not.
		 */
		bool contains(s16 x, s16 y) const;

		/**
		 * Copy the properties of this rect to the destination rect.
		 * @param dest Destination rect to copy to.
		 */
		void copyTo(Rect& dest) const;

		/**
		 * Overloaded & operator.  Returns the intersect of this rectangle and the
		 * rectangle passed as the "rect" argument".
		 * @param rect The rectangle to intersect with this.
		 */
		Rect operator&(const Rect& rect);

		/**
		 * Overloaded + operator.  Returns the smallest rectangle that can contain
		 * this rectangle and the rectangle passed as the "rect" argument".
		 * @param rect The rectangle to add to this.
		 */
		Rect operator+(const Rect& rect);
	};
}

#endif
