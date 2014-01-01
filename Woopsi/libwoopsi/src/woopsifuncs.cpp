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

WoopsiUI::FrameBuffer* frameBuffer[SCREEN_COUNT];
WoopsiUI::GadgetStyle* defaultGadgetStyle;

_pads Pad;
_stylus Stylus;

SDL_Window* _window = NULL;
SDL_Renderer* _renderer = NULL;
SDL_Texture* _texture = NULL;

u16* _bitmaps[SCREEN_COUNT];

SDL_Surface *screen;

void initWoopsiGfxMode() {

	Uint32 initflags = SDL_INIT_VIDEO;

	// Initialize the SDL library
	if (SDL_Init(initflags) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Set video mode
	_window = SDL_CreateWindow("Woopsi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT * 2, 0);
	_renderer = SDL_CreateRenderer(_window, -1, 0);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);

	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT * 2);

	// Create framebuffers
	for (s32 i = 0; i < SCREEN_COUNT; ++i) {
		_bitmaps[i] = new u16[SCREEN_WIDTH * SCREEN_HEIGHT];
	}

	// Bitmaps have ended up in reverse order.
	// TODO: Fix this witchcraft.
	for (s32 i = 0; i < SCREEN_COUNT; ++i) {
		frameBuffer[i] = new WoopsiUI::FrameBuffer(_bitmaps[SCREEN_COUNT - 1 - i], SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	// Initialise default style
	woopsiInitDefaultGadgetStyle();

	// Initialise both arrays
	WoopsiUI::Graphics* graphics;

	for (s32 i = 0; i < SCREEN_COUNT; ++i) {
		graphics = frameBuffer[i]->newGraphics();
		graphics->drawFilledRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		delete graphics;
	}
}

void woopsiVblFunc() {

	SDL_Delay(10);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;

	for (s32 i = 0; i < SCREEN_COUNT; ++i) {
		SDL_UpdateTexture(_texture, &rect, _bitmaps[i], SCREEN_WIDTH * sizeof(u16));

		rect.y += SCREEN_HEIGHT;
	}

	SDL_RenderClear(_renderer);
	SDL_RenderCopy(_renderer, _texture, NULL, NULL);
	SDL_RenderPresent(_renderer);

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
	s32 mouseX = mX;
	s32 mouseY = mY;

	if (SCREEN_COUNT == 2) mouseY -= SCREEN_HEIGHT;

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
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	// Up
	if ((keyState[SDL_SCANCODE_UP]) && ((!Pad.Held.Up) && (!Pad.Newpress.Up))) {
		Pad.Newpress.Up = true;
	} else if ((!keyState[SDL_SCANCODE_UP]) && ((Pad.Held.Up) || (Pad.Newpress.Up))) {
		Pad.Released.Up = true;
		Pad.Held.Up = false;
		Pad.Newpress.Up = false;
		Pad.HeldTime.Up = 0;
	}

	// Down
	if ((keyState[SDL_SCANCODE_DOWN]) && ((!Pad.Held.Down) && (!Pad.Newpress.Down))) {
		Pad.Newpress.Down = true;
	} else if ((!keyState[SDL_SCANCODE_DOWN]) && ((Pad.Held.Down) || (Pad.Newpress.Down))) {
		Pad.Released.Down = true;
		Pad.Held.Down = false;
		Pad.Newpress.Down = false;
		Pad.HeldTime.Down = 0;
	}

	// Left
	if ((keyState[SDL_SCANCODE_LEFT]) && ((!Pad.Held.Left) && (!Pad.Newpress.Left))) {
		Pad.Newpress.Left = true;
	} else if ((!keyState[SDL_SCANCODE_LEFT]) && ((Pad.Held.Left) || (Pad.Newpress.Left))) {
		Pad.Released.Left = true;
		Pad.Held.Left = false;
		Pad.Newpress.Left = false;
		Pad.HeldTime.Left = 0;
	}

	// Right
	if ((keyState[SDL_SCANCODE_RIGHT]) && ((!Pad.Held.Right) && (!Pad.Newpress.Right))) {
		Pad.Newpress.Right = true;
	} else if ((!keyState[SDL_SCANCODE_RIGHT]) && ((Pad.Held.Right) || (Pad.Newpress.Right))) {
		Pad.Released.Right = true;
		Pad.Held.Right = false;
		Pad.Newpress.Right = false;
		Pad.HeldTime.Right = 0;
	}

	// A (assigned as Z on keyboard)
	if ((keyState[SDL_SCANCODE_Z]) && ((!Pad.Held.A) && (!Pad.Newpress.A))) {
		Pad.Newpress.A = true;
	} else if ((!keyState[SDL_SCANCODE_Z]) && ((Pad.Held.A) || (Pad.Newpress.A))) {
		Pad.Released.A = true;
		Pad.Held.A = false;
		Pad.Newpress.A = false;
		Pad.HeldTime.A = 0;
	}

	// B (assigned as X on keyboard)
	if ((keyState[SDL_SCANCODE_X]) && ((!Pad.Held.B) && (!Pad.Newpress.B))) {
		Pad.Newpress.B = true;
	} else if ((!keyState[SDL_SCANCODE_X]) && ((Pad.Held.B) || (Pad.Newpress.B))) {
		Pad.Released.B = true;
		Pad.Held.B = false;
		Pad.Newpress.B = false;
		Pad.HeldTime.B = 0;
	}

	// X (assigned as C on keyboard)
	if ((keyState[SDL_SCANCODE_C]) && ((!Pad.Held.X) && (!Pad.Newpress.X))) {
		Pad.Newpress.X = true;
	} else if ((!keyState[SDL_SCANCODE_C]) && ((Pad.Held.X) || (Pad.Newpress.X))) {
		Pad.Released.X = true;
		Pad.Held.X = false;
		Pad.Newpress.X = false;
		Pad.HeldTime.X = 0;
	}

	// Y (assigned as V on keyboard)
	if ((keyState[SDL_SCANCODE_V]) && ((!Pad.Held.Y) && (!Pad.Newpress.Y))) {
		Pad.Newpress.Y = true;
	} else if ((!keyState[SDL_SCANCODE_V]) && ((Pad.Held.Y) || (Pad.Newpress.Y))) {
		Pad.Released.Y = true;
		Pad.Held.Y = false;
		Pad.Newpress.Y = false;
		Pad.HeldTime.Y = 0;
	}

	// L (assigned as A on keyboard)
	if ((keyState[SDL_SCANCODE_A]) && ((!Pad.Held.L) && (!Pad.Newpress.L))) {
		Pad.Newpress.L = true;
	} else if ((!keyState[SDL_SCANCODE_A]) && ((Pad.Held.L) || (Pad.Newpress.L))) {
		Pad.Released.L = true;
		Pad.Held.L = false;
		Pad.Newpress.L = false;
		Pad.HeldTime.L = 0;
	}

	// R (assigned as S on keyboard)
	if ((keyState[SDL_SCANCODE_S]) && ((!Pad.Held.R) && (!Pad.Newpress.R))) {
		Pad.Newpress.R = true;
	} else if ((!keyState[SDL_SCANCODE_S]) && ((Pad.Held.R) || (Pad.Newpress.R))) {
		Pad.Released.R = true;
		Pad.Held.R = false;
		Pad.Newpress.R = false;
		Pad.HeldTime.R = 0;
	}

	// Start (assigned as D on keyboard)
	if ((keyState[SDL_SCANCODE_D]) && ((!Pad.Held.Start) && (!Pad.Newpress.Start))) {
		Pad.Newpress.Start = true;
	} else if ((!keyState[SDL_SCANCODE_D]) && ((Pad.Held.Start) || (Pad.Newpress.Start))) {
		Pad.Released.Start = true;
		Pad.Held.Start = false;
		Pad.Newpress.Start = false;
		Pad.HeldTime.Start = 0;
	}

	// Select (assigned as F on keyboard)
	if ((keyState[SDL_SCANCODE_F]) && ((!Pad.Held.Select) && (!Pad.Newpress.Select))) {
		Pad.Newpress.Select = true;
	} else if ((!keyState[SDL_SCANCODE_F]) && ((Pad.Held.Select) || (Pad.Newpress.Select))) {
		Pad.Released.Select = true;
		Pad.Held.Select = false;
		Pad.Newpress.Select = false;
		Pad.HeldTime.Select = 0;
	}

	// Exit (assigned as Esc on keyboard)
	if (keyState[SDL_SCANCODE_ESCAPE]) exit(0);

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

WoopsiUI::FrameBuffer* frameBuffer[SCREEN_COUNT];
WoopsiUI::GadgetStyle* defaultGadgetStyle;

_pads Pad;
_stylus Stylus;

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

		// Backup the power
		u16 power_cr = REG_POWERCNT;

		// Shutdown everything
		REG_POWERCNT = 0;

		// Wait for the lid to be opened again
		while (keysHeld() & KEY_LID) {
			swiWaitForVBlank();
		}

		// Return the power
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
	defaultGadgetStyle->colours.text = 0;
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
	for (s32 i = 0; i < SCREEN_COUNT; ++i) {
		delete frameBuffer[i];

#ifdef USING_SDL
		delete _bitmaps[i];
#endif
	}
}
