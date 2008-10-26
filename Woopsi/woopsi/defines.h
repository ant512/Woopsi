#ifndef _DEFINES_H_
#define _DEFINES_H_

/**
 * The TOP_SCREEN_Y_OFFSET specifies the y co-ordinate at which output swaps from the 
 * lower DS screen to the upper screen.  This lets us use a single co-ordinate system
 * instead of one co-ordinate system for each screen.
 */
#define TOP_SCREEN_Y_OFFSET 512

/**
 * Define the width of a screen.
 */
#define SCREEN_WIDTH 256

/**
 * Define the height of a screen.
 */
#define SCREEN_HEIGHT 192

/**
 * The TOP_SCREEN_NUMBER gives the index of the top screen in the DrawBg[] framebuffer
 * pointer array.
 */
#define TOP_SCREEN_NUMBER 1

/**
 * The BOTTOM_SCREEN_NUMBER gives the index of the touch screen in the DrawBg[] framebuffer
 * pointer array.
 */
#define BOTTOM_SCREEN_NUMBER 0

/**
 * Number of VBLs within which a click the user can click again to fire a double-click.
 */
#define DOUBLE_CLICK_TIME 15

/**
 * Value added to click x and y to describe a box within which a click is counted as
 * a double-click.
 */
#define DOUBLE_CLICK_BOUNDS 10

/**
 * Woopsi version number.
 */
#define WOOPSI_VERSION "0.36"

/**
 * Copyright notice.
 */
#define WOOPSI_COPYRIGHT "Copyright 2007-2008 Antony Dzeryn"

/**
 * Ensure NULL is defined.
 */
#ifndef NULL
#define NULL 0
#endif

#endif
