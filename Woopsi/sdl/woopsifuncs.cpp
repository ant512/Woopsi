#include <nds.h>
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

	/* Set 256x384 video mode */
	screen = SDL_SetVideoMode(256, 384, video_bpp, videoflags);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n", video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	DrawBg[0] = new u16[49152];
	DrawBg[1] = new u16[49152];
}

void woopsiVblFunc() {
	//woopsiUpdateInput();
	SDL_PixelFormat* format;
	format = screen->format;
	SDL_LockSurface(screen);
	
	u32 r = 0;
	u32 g = 0;
	u32 b  = 0;
	
	// Draw top screen
	for (u16 i = 0; i < 49152; i++) {
		r = (DrawBg[1][i] & 31) << 3;
		g = (DrawBg[1][i] & (31 << 5)) >> 2;
		b = (DrawBg[1][i] & (31 << 10)) >> 7;
		putPixel(screen, i % SCREEN_WIDTH, (i / SCREEN_WIDTH), SDL_MapRGB(format, r, g, b));
	}
	
	// Draw bottom screen
	for (u16 i = 0; i < 49152; i++) {
		r = (DrawBg[0][i] & 31) << 3;
		g = (DrawBg[0][i] & (31 << 5)) >> 2;
		b = (DrawBg[0][i] & (31 << 10)) >> 7;
		putPixel(screen, i % SCREEN_WIDTH, (i / SCREEN_WIDTH) + 192, SDL_MapRGB(format, r, g, b));
	}
	
	SDL_UnlockSurface(screen);
	SDL_UpdateRect(screen, 0, 0, 256, 384);
	
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
	
	// Check events
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				Stylus.Newpress = true;
				Stylus.Held = true;
				Stylus.Released = false;
				mouseX = event.button.x;
				mouseY = event.button.y - SCREEN_HEIGHT;
				break;
			case SDL_MOUSEBUTTONUP:
				Stylus.Released = true;
				Stylus.Held = false;
				Stylus.Newpress = false;
				mouseX = event.button.x;
				mouseY = event.button.y - SCREEN_HEIGHT;
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y - SCREEN_HEIGHT;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.scancode) {
					case 123:
						Pad.Released.Left = true;
						Pad.Held.Left = false;
						Pad.Newpress.Left = false;
						break;
					case 124:
						Pad.Released.Right = true;
						Pad.Held.Right = false;
						Pad.Newpress.Right = false;
						break;
					case 125:
						Pad.Released.Down = true;
						Pad.Held.Down = false;
						Pad.Newpress.Down = false;
						break;
					case 126:
						Pad.Released.Up = true;
						Pad.Held.Up = false;
						Pad.Newpress.Up = false;
						break;
					case 0:
						Pad.Released.A = true;
						Pad.Held.A = false;
						Pad.Newpress.A = false;
						break;
					case 1:
						Pad.Released.B = true;
						Pad.Held.B = false;
						Pad.Newpress.B = false;
						break;
					case 2:
						Pad.Released.L = true;
						Pad.Held.L = false;
						Pad.Newpress.L = false;
						break;
					case 3:
						Pad.Released.R = true;
						Pad.Held.R = false;
						Pad.Newpress.R = false;
						break;
					case 6:
						Pad.Released.X = true;
						Pad.Held.X = false;
						Pad.Newpress.X = false;
						break;
					case 7:
						Pad.Released.Y = true;
						Pad.Held.Y = false;
						Pad.Newpress.Y = false;
						break;
					case 8:
						Pad.Released.Start = true;
						Pad.Held.Start = false;
						Pad.Newpress.Start = false;
						break;
					case 9:
						Pad.Released.Select = true;
						Pad.Held.Select = false;
						Pad.Newpress.Select = false;
						break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case 123:
						Pad.Newpress.Left = true;
						break;
					case 124:
						Pad.Newpress.Right = true;
						break;
					case 125:
						Pad.Newpress.Down = true;
						break;
					case 126:
						Pad.Newpress.Up = true;
						break;
					case 0:
						Pad.Newpress.A = true;
						break;
					case 1:
						Pad.Newpress.B = true;
						break;
					case 2:
						Pad.Newpress.L = true;
						break;
					case 3:
						Pad.Newpress.R = true;
						break;
					case 6:
						Pad.Newpress.X = true;
						break;
					case 7:
						Pad.Newpress.Y = true;
						break;
					case 8:
						Pad.Newpress.Start = true;
						break;
					case 9:
						Pad.Newpress.Select = true;
						break;
				}
				break;
		}
	}
	
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
