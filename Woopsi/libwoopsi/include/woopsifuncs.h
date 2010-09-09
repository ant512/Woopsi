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
#include "newtopaz.h"
#include "glyphfont.h"
#include "defines.h"

/**
 * Structure to hold the status of the DS' control buttons.
 */
typedef struct PAD {
	union {
		struct {
			u16 A:1;			/**< A button */
			u16 B:1;			/**< B button */
			u16 Select:1;		/**< Select button */
			u16 Start:1;		/**< Start button */
			u16 Right:1;		/**< Right d-pad */
			u16 Left:1;			/**< Left d-pad */
			u16 Up:1;			/**< Up d-pad */
			u16 Down:1;			/**< Down d-pad */
			u16 R:1;			/**< R button */
			u16 L:1;			/**< L button */
			u16 X:1;			/**< X button */
			u16 Y:1;			/**< Y button */
			u16 Touch:1;		/**< Stylus touch */
			u16 Lid:1;			/**< Lid state */
		};
		u16 AllKeys;			/**< Bitmask representing entire control */
	};
} _pad;

/**
 * Struct containing the length of time that each button has been held down.
 */
typedef struct PadHeldTimeStruct {
	u32 A;			/**< A button */
	u32 B;			/**< B button */
	u32 Select;		/**< Select button */
	u32 Start;		/**< Start button */
	u32 Right;		/**< Right button */
	u32 Left;		/**< Left button */
	u32 Up;			/**< Up button */
	u32 Down;		/**< Down button */
	u32 R;			/**< R button */
	u32 L;			/**< L button */
	u32 X;			/**< X button */
	u32 Y;			/**< Y button */
} PadHeldTime;

/**
 * Structure holding a pad struct for each possible button state.
 */
typedef struct PADS {
   _pad Held;					/**< Each value set to 1 represents a held button */
   _pad Released;				/**< Inverse of held */
   _pad Newpress;				/**< Each value set to 1 represents a newly pressed button */
	PadHeldTime HeldTime;		/**< Length of time each button has been held */
} _pads;

/**
 * Struture holding the status of the stylus.
 */
typedef struct STYLUS {
	u8 Held:1;					/**< Stylus is held down */
	u8 Released:1;				/**< Inverse of held */
	u8 Newpress:1;				/**< Stylus has been newly pressed */
	u8 unused:5;				/**< Padding bits */
	s16 X;						/**< X co-ord of the stylus */
	s16 Y;						/**< Y co-ord of the stylus */
	s16 Vx;						/**< X distance the stylus has moved in the last vblank */
	s16 Vy;						/**< Y distance the stylus has moved in the last vblank */
	s16 oldVx;					/**< X co-ord of the stylus at the previous vblank */
	s16 oldVy;					/**< Y co-ord of the stylus at the previous vblank */
	s16 Downtime;				/**< Number of vblanks stylus has been held */
	s16 Uptime;					/**< Number of vblanks stylus has been released */
	s16 DblClick;				/**< Indicates a double click */
} _stylus;

/**
 * Instance of the _pads struct allows access to the pad state.
 */
extern _pads Pad;

/**
 * Instance of the _stylus struct allows access to the stylus state.
 */
extern _stylus Stylus;

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
 * Pointers to the DS' framebuffers.
 */
extern WoopsiUI::FrameBuffer* frameBuffer[2];

/**
 * Pointer to the default gadget style.
 */
extern WoopsiUI::GadgetStyle* defaultGadgetStyle;

/**
 * Initialise the DS' screens into framebuffer mode.  Also sets up some other
 * subsystems and IRQs.
 */
void initWoopsiGfxMode();

/**
 * Initialise the default gadget style.
 */
void woopsiInitDefaultGadgetStyle();

/**
 * Delete the default gadget style.
 */
void woopsiFreeDefaultGadgetStyle();

/**
 * Delete the framebuffer objects.
 */
void woopsiFreeFrameBuffers();

/**
 * Wait for a VBL.  Switches into a wait state if the lid is closed.
 */
void woopsiWaitVBL();

/**
 * Update the pad and stylus structs with the latest physical status.  Called
 * every frame by the VBL function.
 * @see woopsiWaitVBL().
 */
void woopsiUpdateInput();

#endif
