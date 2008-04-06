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
 * Woopsi version number.
 */
#define WOOPSI_VERSION "0.31"

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
