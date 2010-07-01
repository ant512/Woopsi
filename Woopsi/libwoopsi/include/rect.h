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

		Rect();
		Rect(s16 x, s16 y, s32 width, s32 height);
		static Rect* fromDimensions(s16 x, s16 y, s32 width, s32 height);
		static Rect* fromCoordinates(s16 x1, s16 y1, s16 x2, s16 y2);

		inline s16 getX() const { return x; };
		inline s16 getY() const { return y; };
		inline s32 getWidth() const { return width; };
		inline s32 getHeight() const { return height; };

		inline void setX(s16 x) { this->x = x; };
		inline void setY(s16 y) { this->y = y; };
		inline void setWidth(s32 width) { this->width = width; };
		inline void setHeight(s32 height) { this->height = height; };

		void setX2(s16 x2) {

			// Ensure that x contains the smaller value
			if (x2 < x) {
				s16 swap = x;
				x = x2;
				x2 = swap;
			}

			width = (x2 - x) + 1;
		};

		void setY2(s16 y2) {

			// Ensure that y contains the smaller value
			if (y2 < y) {
				s16 swap = y;
				y = y2;
				y2 = swap;
			}

			height = (y2 - y) + 1;
		};

		s16 getX2() const {
			return x + (width - 1);
		};

		s16 getY2() const {
			return y + (height - 1);
		};

		/**
		 * Populates dest with a rectangle representating the intersection
		 * of this rectangle and rect.
		 * @param rect The rectangle to intersect with this.
		 * @param dest The destination rectangle.
		 */
		void intersect(const Rect& rect, Rect& dest) const;

		/**
		 * Populates dest with a rectangle representating the smallest
		 * rectangle that contains this rectangle and rect.
		 * @param rect The rectangle to add to this.
		 * @param dest The destination rectangle.
		 */
		void add(const Rect& rect, Rect& dest) const;

		Rect operator&(const Rect& rect);
		Rect operator+(const Rect& rect);
	};
}

#endif
