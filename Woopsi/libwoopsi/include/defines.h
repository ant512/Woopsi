#ifndef _DEFINES_H_
#define _DEFINES_H_

/**
 * Number of screens available.  Currently supported options are 1 or 2.
 */
const s32 SCREEN_COUNT = 2;

/**
 * The TOP_SCREEN_Y_OFFSET specifies the y co-ordinate at which output swaps
 * from the lower DS screen to the upper screen.  This lets us use a single
 * co-ordinate system instead of one co-ordinate system for each display.  The
 * offset is larger than the physical screen because we can drag screens down.
 * Rather than try to deal with this strange edge case it is easier to simply
 * add some "dead space" between the top and bottom displays.
 */
const s32 TOP_SCREEN_Y_OFFSET = 512;

/**
 * The TOP_SCREEN_NUMBER gives the index of the top screen in the frameBuffer[]
 * framebuffer pointer array.
 */
const s32 TOP_SCREEN_NUMBER = 1;

/**
 * The BOTTOM_SCREEN_NUMBER gives the index of the touch screen in the
 * frameBuffer[] framebuffer pointer array.
 */
const s32 BOTTOM_SCREEN_NUMBER = 0;

/**
 * Value added to click x and y to describe a box within which a click is
 * counted as a double-click.
 */
const s32 DOUBLE_CLICK_BOUNDS = 10;

#ifndef USING_SDL

/**
 * Number of VBLs within which a click the user can click again to fire a
 * double-click.
 */
const s32 DOUBLE_CLICK_TIME = 45;

/**
 * Time taken before a key (either d-pad or keyboard) starts repeating.
 */
const s32 KEY_INITIAL_REPEAT_TIME = 25;

/**
 * Time taken before a repeating key repeats again.
 */
const s32 KEY_SECONDARY_REPEAT_TIME = 5;

#else

/**
 * Number of VBLs within which the user can click again to fire a
 * double-click.
 */
const s32 DOUBLE_CLICK_TIME = 8000;

/**
 * Time taken before a key (either d-pad or keyboard) starts repeating.
 */
const s32 KEY_INITIAL_REPEAT_TIME = 4000;

/**
 * Time taken before a repeating key repeats again.
 */
const s32 KEY_SECONDARY_REPEAT_TIME = 1000;

#endif

/**
 * Woopsi version number.
 */
const char WOOPSI_VERSION[] = "0.99.6 Beta";

/**
 * Copyright notice.
 */
const char WOOPSI_COPYRIGHT[] = "Copyright 2007-2010 Antony Dzeryn";

/**
 * Ensure NULL is defined.
 */
#ifndef NULL
#define NULL 0
#endif

#endif
