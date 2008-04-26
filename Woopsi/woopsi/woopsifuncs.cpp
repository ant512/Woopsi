/* Contributed by Steven
 * 20071213
 * - Minor modifications and SDL code by Ant
 */

#include <nds.h>

#ifdef USING_SDL

#include <string.h>
#include "woopsifuncs.h"
#include "defines.h"

u16* DrawBg[2];

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

	/* Set 320x480 video mode */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT * 2, video_bpp, videoflags);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	DrawBg[0] = new u16[SCREEN_WIDTH * SCREEN_HEIGHT];
	DrawBg[1] = new u16[SCREEN_WIDTH * SCREEN_HEIGHT];
}

void woopsiVblFunc() {
	//woopsiUpdateInput();
	SDL_PixelFormat* format;
	format = screen->format;
	SDL_LockSurface(screen);

	u32 r = 0;
	u32 g = 0;
	u32 b = 0;

	// Draw top screen
	for (u32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		r = (DrawBg[1][i] & 31) << 3;
		g = (DrawBg[1][i] & (31 << 5)) >> 2;
		b = (DrawBg[1][i] & (31 << 10)) >> 7;
		putPixel(screen, i % SCREEN_WIDTH, (i / SCREEN_WIDTH), SDL_MapRGB(format, r, g, b));
	}

	// Draw bottom screen
	for (u32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		r = (DrawBg[0][i] & 31) << 3;
		g = (DrawBg[0][i] & (31 << 5)) >> 2;
		b = (DrawBg[0][i] & (31 << 10)) >> 7;
		putPixel(screen, i % SCREEN_WIDTH, (i / SCREEN_WIDTH) + SCREEN_HEIGHT, SDL_MapRGB(format, r, g, b));
	}

	SDL_UnlockSurface(screen);
	SDL_UpdateRect(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 2);

	if (Stylus.Newpress) {
		Stylus.Held = true;
		Stylus.Newpress = false;
	}

	Stylus.Released = false;

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
	}
	
	// Down
	if ((keyState[SDLK_DOWN]) && ((!Pad.Held.Down) && (!Pad.Newpress.Down))) {
		Pad.Newpress.Down = true;
	} else if ((!keyState[SDLK_DOWN]) && ((Pad.Held.Down) || (Pad.Newpress.Down))) {
		Pad.Released.Down = true;
		Pad.Held.Down = false;
		Pad.Newpress.Down = false;
	}
	
	// Left
	if ((keyState[SDLK_LEFT]) && ((!Pad.Held.Left) && (!Pad.Newpress.Left))) {
		Pad.Newpress.Left = true;
	} else if ((!keyState[SDLK_LEFT]) && ((Pad.Held.Left) || (Pad.Newpress.Left))) {
		Pad.Released.Left = true;
		Pad.Held.Left = false;
		Pad.Newpress.Left = false;
	}
	
	// Right
	if ((keyState[SDLK_RIGHT]) && ((!Pad.Held.Right) && (!Pad.Newpress.Right))) {
		Pad.Newpress.Right = true;
	} else if ((!keyState[SDLK_RIGHT]) && ((Pad.Held.Right) || (Pad.Newpress.Right))) {
		Pad.Released.Right = true;
		Pad.Held.Right = false;
		Pad.Newpress.Right = false;
	}	

	// A (assigned as Z on keyboard)
	if ((keyState[SDLK_z]) && ((!Pad.Held.A) && (!Pad.Newpress.A))) {
		Pad.Newpress.A = true;
	} else if ((!keyState[SDLK_z]) && ((Pad.Held.A) || (Pad.Newpress.A))) {
		Pad.Released.A = true;
		Pad.Held.A = false;
		Pad.Newpress.A = false;
	}
	
	// B (assigned as X on keyboard)
	if ((keyState[SDLK_x]) && ((!Pad.Held.B) && (!Pad.Newpress.B))) {
		Pad.Newpress.B = true;
	} else if ((!keyState[SDLK_x]) && ((Pad.Held.B) || (Pad.Newpress.B))) {
		Pad.Released.B = true;
		Pad.Held.B = false;
		Pad.Newpress.B = false;
	}
	
	// X (assigned as C on keyboard)
	if ((keyState[SDLK_c]) && ((!Pad.Held.X) && (!Pad.Newpress.X))) {
		Pad.Newpress.X = true;
	} else if ((!keyState[SDLK_c]) && ((Pad.Held.X) || (Pad.Newpress.X))) {
		Pad.Released.X = true;
		Pad.Held.X = false;
		Pad.Newpress.X = false;
	}
	
	// Y (assigned as V on keyboard)
	if ((keyState[SDLK_v]) && ((!Pad.Held.Y) && (!Pad.Newpress.Y))) {
		Pad.Newpress.Y = true;
	} else if ((!keyState[SDLK_v]) && ((Pad.Held.Y) || (Pad.Newpress.Y))) {
		Pad.Released.Y = true;
		Pad.Held.Y = false;
		Pad.Newpress.Y = false;
	}
	
	// L (assigned as A on keyboard)
	if ((keyState[SDLK_a]) && ((!Pad.Held.L) && (!Pad.Newpress.L))) {
		Pad.Newpress.L = true;
	} else if ((!keyState[SDLK_a]) && ((Pad.Held.L) || (Pad.Newpress.L))) {
		Pad.Released.L = true;
		Pad.Held.L = false;
		Pad.Newpress.L = false;
	}
	
	// R (assigned as S on keyboard)
	if ((keyState[SDLK_s]) && ((!Pad.Held.R) && (!Pad.Newpress.R))) {
		Pad.Newpress.R = true;
	} else if ((!keyState[SDLK_s]) && ((Pad.Held.R) || (Pad.Newpress.R))) {
		Pad.Released.R = true;
		Pad.Held.R = false;
		Pad.Newpress.R = false;
	}
	
	// Start (assigned as D on keyboard)
	if ((keyState[SDLK_d]) && ((!Pad.Held.Start) && (!Pad.Newpress.Start))) {
		Pad.Newpress.Start = true;
	} else if ((!keyState[SDLK_d]) && ((Pad.Held.Start) || (Pad.Newpress.Start))) {
		Pad.Released.Start = true;
		Pad.Held.Start = false;
		Pad.Newpress.Start = false;
	}

	// Select (assigned as F on keyboard)
	if ((keyState[SDLK_f]) && ((!Pad.Held.Select) && (!Pad.Newpress.Select))) {
		Pad.Newpress.Select = true;
	} else if ((!keyState[SDLK_f]) && ((Pad.Held.Select) || (Pad.Newpress.Select))) {
		Pad.Released.Select = true;
		Pad.Held.Select = false;
		Pad.Newpress.Select = false;
	}
	
	// Update other stylus properties
	Stylus.DblClick = Stylus.Newpress && (Stylus.Downtime+Stylus.Uptime < 45);
	Stylus.Downtime *= !Stylus.Newpress; // = 0 if newpress
	Stylus.Downtime += Stylus.Held;

	Stylus.Uptime *= !Stylus.Released; // = 0 when released
	Stylus.Uptime += !Stylus.Held;

	if (Stylus.Held) {
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

void DMA_Copy(u16* source, u16* dest, u32 count, u32 mode) {
	for (u32 i = 0; i < count; i++) {
		*(dest + i) = *(source + i);
	}
}

void DMA_Force(u16 source, u16* dest, u32 count, u32 mode) {
	for (u32 i = 0; i < count; i++) {
		*(dest + i) = source;
	}
}

void DMA_Copy2(u16* source, u16* dest, u32 count, u32 mode) {
	DMA_Copy(source, dest, count, mode);
}

void DMA_Force2(u16 source, u16* dest, u32 count, u32 mode) {
	DMA_Force(source, dest, count, mode);
}

bool DMA_Active() {
	return false;
}

bool DMA_Active2() {
	return false;
}

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

// Function does nothing, as mem does not need flushing in SDL
void DC_FlushRange(const void *base, u32 size) {
}

#else

#include <string.h>
#include "woopsifuncs.h"

u16* DrawBg[2];

// Are we using PALib?
#ifndef USING_PALIB

// No PALib, so use LibNDS

_pads Pad;
_stylus Stylus;

bool woopsiLidClosed() {
	return Pad.Held.Lid;
}

void woopsiUpdateInput() {

	touchPosition touch;

	// Get the state of the keys
	scanKeys();
	Pad.Newpress.AllKeys = keysDown();
	Pad.Held.AllKeys = keysHeld();
	Pad.Released.AllKeys = keysUp();

	// Deal with the Stylus.
	touch = touchReadXY(); 

	Stylus.Newpress = Pad.Newpress.Touch;
	Stylus.Held     = Pad.Held.Touch;
	Stylus.Released = Pad.Released.Touch;

	Stylus.DblClick = Stylus.Newpress && (Stylus.Downtime+Stylus.Uptime < 45);
	Stylus.Downtime *= !Stylus.Newpress; // = 0 if newpress
	Stylus.Downtime += Stylus.Held;

	Stylus.Uptime *= !Stylus.Released; // = 0 when released
	Stylus.Uptime += !Stylus.Held;

	if (Stylus.Held) {
		if(Stylus.Newpress){
			Stylus.Vx = Stylus.oldVx = 0;
			Stylus.Vy = Stylus.oldVy = 0;
		}
		else{
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
	powerON(POWER_ALL_2D);

	irqInit();
	irqSet(IRQ_VBLANK, woopsiVblFunc);
	irqEnable(IRQ_VBLANK);

	videoSetMode( MODE_5_2D | DISPLAY_BG3_ACTIVE );
	videoSetModeSub( MODE_5_2D | DISPLAY_BG3_ACTIVE );	//sub bg 0 will be used to print text

	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankC( VRAM_C_SUB_BG );

	BG3_CR = BG_BMP16_256x256 | BG_BMP_BASE(0);
	
	BG3_XDY = 0;
	BG3_XDX = 1 << 8;
	BG3_YDX = 0;
	BG3_YDY = 1 << 8;

	SUB_BG3_CR = BG_BMP16_256x256 | BG_BMP_BASE(0);
	
	SUB_BG3_XDY = 0;
	SUB_BG3_XDX = 1 << 8;
	SUB_BG3_YDX = 0;
	SUB_BG3_YDY = 1 << 8;

	DrawBg[1] = (u16*)BG_BMP_RAM(0);
	DrawBg[0] = (u16*)BG_BMP_RAM_SUB(0);

	memset( &Stylus, 0, sizeof(_stylus) );
}

void woopsiWaitVBL() {
	if (keysHeld() & KEY_LID) {

		u16 power_cr = REG_POWERCNT; // backup the power...
		REG_POWERCNT = 0; // Shutdown everything :p
		
		// Wait for the lid to be opened again...
		while (keysHeld() & KEY_LID)
			swiWaitForVBlank();
		
		// Return the power !
		REG_POWERCNT = power_cr;
	}
	swiWaitForVBlank();
}

#else

// Using PALib

bool woopsiLidClosed() {
	return PA_LidClosed();
}

void initWoopsiGfxMode() {
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	// Initialise the screens
	PA_Init16bitBg(0, 0);
	PA_Init16bitBg(1, 0);

	// Set up the framebuffer pointers
	DrawBg[1] = PA_DrawBg[1];
	DrawBg[0] = PA_DrawBg[0];
}

void woopsiWaitVBL() {
	PA_WaitForVBL();
}

#endif
#endif
