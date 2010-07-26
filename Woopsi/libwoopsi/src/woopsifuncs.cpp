/* Contributed by Steven
 * 20071213
 * - Minor modifications and SDL code by Ant
 */

#include <nds.h>
#include <string.h>
#include "woopsifuncs.h"
#include "graphics.h"

#ifdef USING_SDL

#include "defines.h"

WoopsiUI::FrameBuffer* frameBuffer[2];
WoopsiUI::GadgetStyle* defaultGadgetStyle;

_pads Pad;
_stylus Stylus;

Uint32 initflags = SDL_INIT_VIDEO;
SDL_Surface *screen;
Uint8 video_bpp = 0;
Uint32 videoflags = SDL_SWSURFACE;
SDL_Event event;
s32 mouseX;
s32 mouseY;

bool woopsiLidClosed() {
	return Pad.Held.Lid;
}

void initWoopsiGfxMode() {

	initflags = SDL_INIT_VIDEO;
	video_bpp = 0;
	videoflags = SDL_SWSURFACE;

	// Initialize the SDL library
	if (SDL_Init(initflags) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Set video mode
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT * 2, video_bpp, videoflags);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	// Create framebuffers
	frameBuffer[0] = new WoopsiUI::FrameBuffer(screen, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_HEIGHT);
	frameBuffer[1] = new WoopsiUI::FrameBuffer(screen, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	// Initialise default style
	woopsiInitDefaultGadgetStyle();

	// Initialise both arrays
	WoopsiUI::Graphics* graphics = frameBuffer[0]->newGraphics();
	graphics->drawFilledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	delete graphics;

	graphics = frameBuffer[1]->newGraphics();
	graphics->drawFilledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	delete graphics;
}

void woopsiVblFunc() {
	SDL_UpdateRect(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 2);

	if (Stylus.Newpress) {
		Stylus.Held = true;
		Stylus.Newpress = false;
	}

	Stylus.Released = false;
	
	if (Pad.Held.Left) Pad.HeldTime.Left++;
	if (Pad.Held.Right) Pad.HeldTime.Right++;
	if (Pad.Held.Up) Pad.HeldTime.Up++;
	if (Pad.Held.Down) Pad.HeldTime.Down++;
	if (Pad.Held.A) Pad.HeldTime.A++;
	if (Pad.Held.B) Pad.HeldTime.B++;
	if (Pad.Held.X) Pad.HeldTime.X++;
	if (Pad.Held.Y) Pad.HeldTime.Y++;
	if (Pad.Held.Start) Pad.HeldTime.Start++;
	if (Pad.Held.Select) Pad.HeldTime.Select++;
	if (Pad.Held.L) Pad.HeldTime.L++;
	if (Pad.Held.R) Pad.HeldTime.R++;

	if (Pad.Newpress.Left) Pad.Held.Left = true;
	if (Pad.Newpress.Right) Pad.Held.Right = true;
	if (Pad.Newpress.Up) Pad.Held.Up = true;
	if (Pad.Newpress.Down) Pad.Held.Down = true;
	if (Pad.Newpress.A) Pad.Held.A = true;
	if (Pad.Newpress.B) Pad.Held.B = true;
	if (Pad.Newpress.X) Pad.Held.X = true;
	if (Pad.Newpress.Y) Pad.Held.Y = true;
	if (Pad.Newpress.Start) Pad.Held.Start = true;
	if (Pad.Newpress.Select) Pad.Held.Select = true;
	if (Pad.Newpress.L) Pad.Held.L = true;
	if (Pad.Newpress.R) Pad.Held.R = true;

	Pad.Released.Left = false;
	Pad.Released.Right = false;
	Pad.Released.Up = false;
	Pad.Released.Down = false;
	Pad.Released.A = false;
	Pad.Released.B = false;
	Pad.Released.X = false;
	Pad.Released.Y = false;
	Pad.Released.L = false;
	Pad.Released.R = false;
	Pad.Released.Start = false;
	Pad.Released.Select = false;

	Pad.Newpress.Left = false;
	Pad.Newpress.Right = false;
	Pad.Newpress.Up = false;
	Pad.Newpress.Down = false;
	Pad.Newpress.A = false;
	Pad.Newpress.B = false;
	Pad.Newpress.X = false;
	Pad.Newpress.Y = false;
	Pad.Newpress.L = false;
	Pad.Newpress.R = false;
	Pad.Newpress.Start = false;
	Pad.Newpress.Select = false;
	
	// Read mouse state
	int mState;
	int mX;
	int mY;
	
	mState = SDL_GetMouseState(&mX, &mY);
	
	// Update mouse position
	mouseX = mX;
	mouseY = mY - SCREEN_HEIGHT;
	
	// Check buttons
	if ((mState & SDL_BUTTON_LEFT) && (!Stylus.Held)) {
		
		// New click
		Stylus.Newpress = true;
		Stylus.Held = true;
		Stylus.Released = false;

	} else if ((!(mState & SDL_BUTTON_LEFT)) && (Stylus.Held)) {
		
		// Release
		Stylus.Released = true;
		Stylus.Held = false;
		Stylus.Newpress = false;
	}
	
	// Get key state
    Uint8* keyState = SDL_GetKeyState(NULL);

	// Up
	if ((keyState[SDLK_UP]) && ((!Pad.Held.Up) && (!Pad.Newpress.Up))) {
		Pad.Newpress.Up = true;
	} else if ((!keyState[SDLK_UP]) && ((Pad.Held.Up) || (Pad.Newpress.Up))) {
		Pad.Released.Up = true;
		Pad.Held.Up = false;
		Pad.Newpress.Up = false;
		Pad.HeldTime.Up = 0;
	}
	
	// Down
	if ((keyState[SDLK_DOWN]) && ((!Pad.Held.Down) && (!Pad.Newpress.Down))) {
		Pad.Newpress.Down = true;
	} else if ((!keyState[SDLK_DOWN]) && ((Pad.Held.Down) || (Pad.Newpress.Down))) {
		Pad.Released.Down = true;
		Pad.Held.Down = false;
		Pad.Newpress.Down = false;
		Pad.HeldTime.Down = 0;
	}
	
	// Left
	if ((keyState[SDLK_LEFT]) && ((!Pad.Held.Left) && (!Pad.Newpress.Left))) {
		Pad.Newpress.Left = true;
	} else if ((!keyState[SDLK_LEFT]) && ((Pad.Held.Left) || (Pad.Newpress.Left))) {
		Pad.Released.Left = true;
		Pad.Held.Left = false;
		Pad.Newpress.Left = false;
		Pad.HeldTime.Left = 0;
	}
	
	// Right
	if ((keyState[SDLK_RIGHT]) && ((!Pad.Held.Right) && (!Pad.Newpress.Right))) {
		Pad.Newpress.Right = true;
	} else if ((!keyState[SDLK_RIGHT]) && ((Pad.Held.Right) || (Pad.Newpress.Right))) {
		Pad.Released.Right = true;
		Pad.Held.Right = false;
		Pad.Newpress.Right = false;
		Pad.HeldTime.Right = 0;
	}	

	// A (assigned as Z on keyboard)
	if ((keyState[SDLK_z]) && ((!Pad.Held.A) && (!Pad.Newpress.A))) {
		Pad.Newpress.A = true;
	} else if ((!keyState[SDLK_z]) && ((Pad.Held.A) || (Pad.Newpress.A))) {
		Pad.Released.A = true;
		Pad.Held.A = false;
		Pad.Newpress.A = false;
		Pad.HeldTime.A = 0;
	}
	
	// B (assigned as X on keyboard)
	if ((keyState[SDLK_x]) && ((!Pad.Held.B) && (!Pad.Newpress.B))) {
		Pad.Newpress.B = true;
	} else if ((!keyState[SDLK_x]) && ((Pad.Held.B) || (Pad.Newpress.B))) {
		Pad.Released.B = true;
		Pad.Held.B = false;
		Pad.Newpress.B = false;
		Pad.HeldTime.B = 0;
	}
	
	// X (assigned as C on keyboard)
	if ((keyState[SDLK_c]) && ((!Pad.Held.X) && (!Pad.Newpress.X))) {
		Pad.Newpress.X = true;
	} else if ((!keyState[SDLK_c]) && ((Pad.Held.X) || (Pad.Newpress.X))) {
		Pad.Released.X = true;
		Pad.Held.X = false;
		Pad.Newpress.X = false;
		Pad.HeldTime.X = 0;
	}
	
	// Y (assigned as V on keyboard)
	if ((keyState[SDLK_v]) && ((!Pad.Held.Y) && (!Pad.Newpress.Y))) {
		Pad.Newpress.Y = true;
	} else if ((!keyState[SDLK_v]) && ((Pad.Held.Y) || (Pad.Newpress.Y))) {
		Pad.Released.Y = true;
		Pad.Held.Y = false;
		Pad.Newpress.Y = false;
		Pad.HeldTime.Y = 0;
	}
	
	// L (assigned as A on keyboard)
	if ((keyState[SDLK_a]) && ((!Pad.Held.L) && (!Pad.Newpress.L))) {
		Pad.Newpress.L = true;
	} else if ((!keyState[SDLK_a]) && ((Pad.Held.L) || (Pad.Newpress.L))) {
		Pad.Released.L = true;
		Pad.Held.L = false;
		Pad.Newpress.L = false;
		Pad.HeldTime.L = 0;
	}
	
	// R (assigned as S on keyboard)
	if ((keyState[SDLK_s]) && ((!Pad.Held.R) && (!Pad.Newpress.R))) {
		Pad.Newpress.R = true;
	} else if ((!keyState[SDLK_s]) && ((Pad.Held.R) || (Pad.Newpress.R))) {
		Pad.Released.R = true;
		Pad.Held.R = false;
		Pad.Newpress.R = false;
		Pad.HeldTime.R = 0;
	}
	
	// Start (assigned as D on keyboard)
	if ((keyState[SDLK_d]) && ((!Pad.Held.Start) && (!Pad.Newpress.Start))) {
		Pad.Newpress.Start = true;
	} else if ((!keyState[SDLK_d]) && ((Pad.Held.Start) || (Pad.Newpress.Start))) {
		Pad.Released.Start = true;
		Pad.Held.Start = false;
		Pad.Newpress.Start = false;
		Pad.HeldTime.Start = 0;
	}

	// Select (assigned as F on keyboard)
	if ((keyState[SDLK_f]) && ((!Pad.Held.Select) && (!Pad.Newpress.Select))) {
		Pad.Newpress.Select = true;
	} else if ((!keyState[SDLK_f]) && ((Pad.Held.Select) || (Pad.Newpress.Select))) {
		Pad.Released.Select = true;
		Pad.Held.Select = false;
		Pad.Newpress.Select = false;
		Pad.HeldTime.Select = 0;
	}

	// Exit (assigned as Esc on keyboard)
	if (keyState[SDLK_ESCAPE]) exit(0);
	
	// Update other stylus properties
	Stylus.Downtime *= !Stylus.Newpress; // = 0 if newpress
	Stylus.Downtime += Stylus.Held;

	Stylus.Uptime *= !Stylus.Released; // = 0 when released
	Stylus.Uptime += !Stylus.Held;

	if (Stylus.Held) {

		Stylus.DblClick = Stylus.Newpress && (Stylus.Downtime + Stylus.Uptime < DOUBLE_CLICK_TIME);

		if (Stylus.Newpress) {
			Stylus.Vx = Stylus.oldVx = 0;
			Stylus.Vy = Stylus.oldVy = 0;
		} else {
			Stylus.oldVx = Stylus.Vx;
			Stylus.oldVy = Stylus.Vy;
			Stylus.Vx = mouseX - Stylus.X;
			Stylus.Vy = mouseY - Stylus.Y;
		}

		Stylus.X = mouseX;
		Stylus.Y = mouseY;
	}
}

void woopsiWaitVBL() {
	woopsiVblFunc();
}

int fatInitDefault() { return 1; }

#else

// Using libnds

WoopsiUI::FrameBuffer* frameBuffer[2];
WoopsiUI::GadgetStyle* defaultGadgetStyle;

_pads Pad;
_stylus Stylus;

bool woopsiLidClosed() {
	return Pad.Held.Lid;
}

void woopsiUpdateInput() {

	touchPosition touch;
	
	// Update held timers
	if (Pad.Held.Left) Pad.HeldTime.Left++;
	if (Pad.Held.Right) Pad.HeldTime.Right++;
	if (Pad.Held.Up) Pad.HeldTime.Up++;
	if (Pad.Held.Down) Pad.HeldTime.Down++;
	if (Pad.Held.A) Pad.HeldTime.A++;
	if (Pad.Held.B) Pad.HeldTime.B++;
	if (Pad.Held.X) Pad.HeldTime.X++;
	if (Pad.Held.Y) Pad.HeldTime.Y++;
	if (Pad.Held.Start) Pad.HeldTime.Start++;
	if (Pad.Held.Select) Pad.HeldTime.Select++;
	if (Pad.Held.L) Pad.HeldTime.L++;
	if (Pad.Held.R) Pad.HeldTime.R++;
	
	// Get the state of the keys
	scanKeys();
	Pad.Newpress.AllKeys = keysDown();
	Pad.Held.AllKeys = keysHeld();
	Pad.Released.AllKeys = keysUp();
	
	// Update held timers
	if (!Pad.Held.Left) Pad.HeldTime.Left = 0;
	if (!Pad.Held.Right) Pad.HeldTime.Right = 0;
	if (!Pad.Held.Up) Pad.HeldTime.Up = 0;
	if (!Pad.Held.Down) Pad.HeldTime.Down = 0;
	if (!Pad.Held.A) Pad.HeldTime.A = 0;
	if (!Pad.Held.B) Pad.HeldTime.B = 0;
	if (!Pad.Held.X) Pad.HeldTime.X = 0;
	if (!Pad.Held.Y) Pad.HeldTime.Y = 0;
	if (!Pad.Held.Start) Pad.HeldTime.Start = 0;
	if (!Pad.Held.Select) Pad.HeldTime.Select = 0;
	if (!Pad.Held.L) Pad.HeldTime.L = 0;
	if (!Pad.Held.R) Pad.HeldTime.R = 0;

	// Deal with the Stylus.
	touchRead(&touch);

	Stylus.Newpress = Pad.Newpress.Touch;
	Stylus.Held     = Pad.Held.Touch;
	Stylus.Released = Pad.Released.Touch;

	Stylus.DblClick = Stylus.Newpress && (Stylus.Downtime + Stylus.Uptime < DOUBLE_CLICK_TIME);
	Stylus.Downtime *= !Stylus.Newpress; // = 0 if newpress
	Stylus.Downtime += Stylus.Held;

	Stylus.Uptime *= !Stylus.Released; // = 0 when released
	Stylus.Uptime += !Stylus.Held;

	if (Stylus.Held) {
		if (Stylus.Newpress) {
			Stylus.Vx = Stylus.oldVx = 0;
			Stylus.Vy = Stylus.oldVy = 0;
		} else{
			Stylus.oldVx = Stylus.Vx;
			Stylus.oldVy = Stylus.Vy;
			Stylus.Vx = touch.px - Stylus.X;
			Stylus.Vy = touch.py - Stylus.Y;
		}
		
		Stylus.X = touch.px;
		Stylus.Y = touch.py;
	}
}

void woopsiVblFunc() {
	woopsiUpdateInput();
}

void initWoopsiGfxMode() {
	powerOn(POWER_ALL_2D);

	irqSet(IRQ_VBLANK, woopsiVblFunc);

	videoSetMode( MODE_5_2D | DISPLAY_BG3_ACTIVE );
	videoSetModeSub( MODE_5_2D | DISPLAY_BG3_ACTIVE );

	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankC( VRAM_C_SUB_BG );

	// Initialise backgrounds
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	frameBuffer[1] = new WoopsiUI::FrameBuffer((u16*)BG_BMP_RAM(0), SCREEN_WIDTH, SCREEN_HEIGHT);
	frameBuffer[0] = new WoopsiUI::FrameBuffer((u16*)BG_BMP_RAM_SUB(0), SCREEN_WIDTH, SCREEN_HEIGHT);

	woopsiInitDefaultGadgetStyle();
	
	memset( &Stylus, 0, sizeof(_stylus) );
}

void woopsiWaitVBL() {
	if (keysHeld() & KEY_LID) {
		u16 power_cr = REG_POWERCNT; // backup the power...
		REG_POWERCNT = 0; // Shutdown everything :p

		// Wait for the lid to be opened again...
		while (keysHeld() & KEY_LID) {
			swiWaitForVBlank();
		}
		
		// Return the power !
		REG_POWERCNT = power_cr;
	}
	swiWaitForVBlank();
}

#endif

void woopsiInitDefaultGadgetStyle() {
	defaultGadgetStyle = new WoopsiUI::GadgetStyle();

	defaultGadgetStyle->colours.back = woopsiRGB(20, 20, 20);
	defaultGadgetStyle->colours.shine = woopsiRGB(31, 31, 31);
	defaultGadgetStyle->colours.highlight = woopsiRGB(12, 17, 23);
	defaultGadgetStyle->colours.shadow = woopsiRGB(0, 0, 0);
	defaultGadgetStyle->colours.fill = woopsiRGB(24, 24, 24);
	defaultGadgetStyle->colours.dark = woopsiRGB(15, 15, 15);
	defaultGadgetStyle->font = new WoopsiUI::NewTopaz();
	defaultGadgetStyle->glyphFont = new WoopsiUI::GlyphFont();
}

void woopsiFreeDefaultGadgetStyle() {
	delete defaultGadgetStyle->font;
	delete defaultGadgetStyle->glyphFont;
	delete defaultGadgetStyle;
}

void woopsiFreeFrameBuffers() {

	// Delete the framebuffers
	delete frameBuffer[0];
	delete frameBuffer[1];
}
