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
 * The TOP_SCREEN_NUMBER gives the index of the top screen in the frameBuffer[] framebuffer
 * pointer array.
 */
#define TOP_SCREEN_NUMBER 1

/**
 * The BOTTOM_SCREEN_NUMBER gives the index of the touch screen in the frameBuffer[] framebuffer
 * pointer array.
 */
#define BOTTOM_SCREEN_NUMBER 0

/**
 * Number of VBLs within which a click the user can click again to fire a double-click.
 */
#define DOUBLE_CLICK_TIME 45

/**
 * Value added to click x and y to describe a box within which a click is counted as
 * a double-click.
 */
#define DOUBLE_CLICK_BOUNDS 10

/**
 * Time taken before a key (either d-pad or keyboard) starts repeating.
 */
#define KEY_INITIAL_REPEAT_TIME 25

/**
 * Time taken before a repeating key repeats again.
 */
#define KEY_SECONDARY_REPEAT_TIME 5

/**
 * Woopsi version number.
 */
#define WOOPSI_VERSION "0.99.1 Beta"

/**
 * Copyright notice.
 */
#define WOOPSI_COPYRIGHT "Copyright 2007-2010 Antony Dzeryn"

/**
 * Ensure NULL is defined.
 */
#ifndef NULL
#define NULL 0
#endif

#endif
