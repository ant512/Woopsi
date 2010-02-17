#ifndef _RECT_H_
#define _RECT_H_

namespace WoopsiUI {

	/**
	 * Struct describing a rectangle.
	 */
	typedef struct {
		s16 x;							/**< X co-ordinate of the rectangle. */
		s16 y;							/**< Y co-ordinate of the rectangle. */
		s32 width;						/**< Width of the rectangle. */
		s32 height;						/**< Height of the rectangle. */
	} Rect;
}

#endif
