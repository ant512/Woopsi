/* Contributed by Steven
 * 20071213
 * - Minor modifications and SDL code by Ant
 */

#ifndef _WOOPSIFUNCS_H_
#define _WOOPSIFUNCS_H_

#include <nds.h>
#include "framebuffer.h"
#include "bitmapwrapper.h"
#include "gadgetstyle.h"
#include "fonts/newtopaz.h"
#include "fonts/glyphfont.h"
#include "defines.h"

#ifdef USING_SDL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#else

#include <nds/memory.h>
#include <nds/bios.h>

#endif

/**
 * Pointer to the default gadget style.
 */
extern WoopsiUI::GadgetStyle* defaultGadgetStyle;

/**
 * Initialise the default gadget style.
 */
void woopsiInitDefaultGadgetStyle();

/**
 * Delete the default gadget style.
 */
void woopsiFreeDefaultGadgetStyle();

#endif
