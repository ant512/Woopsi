#include "hardware.h"

using namespace WoopsiUI;

Pad Hardware::_pad;
Stylus Hardware::_stylus;

FrameBuffer* Hardware::_topBuffer = NULL;
FrameBuffer* Hardware::_bottomBuffer = NULL;

WoopsiUI::Graphics* Hardware::_topGfx = NULL;
WoopsiUI::Graphics* Hardware::_bottomGfx = NULL;

#ifdef USING_SDL

SDL_Window* Hardware::_window = NULL;
SDL_Renderer* Hardware::_renderer = NULL;
SDL_Texture* Hardware::_texture = NULL;

u16* Hardware::_topBitmap = NULL;
u16* Hardware::_bottomBitmap = NULL;

#endif

void Hardware::init() {

#ifndef USING_SDL

	powerOn(POWER_ALL_2D);

	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);
	videoSetModeSub(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankB(VRAM_B_MAIN_BG_0x06020000);
	vramSetBankC(VRAM_C_SUB_BG);

	// Initialise backgrounds
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	_topBuffer = new FrameBuffer((u16*)BG_BMP_RAM(0), SCREEN_WIDTH, SCREEN_HEIGHT);
	_bottomBuffer = new FrameBuffer((u16*)BG_BMP_RAM_SUB(0), SCREEN_WIDTH, SCREEN_HEIGHT);

#else

	Uint32 initflags = SDL_INIT_VIDEO;

	// Initialize the SDL library
	if (SDL_Init(initflags) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// Set video mode
    _window = SDL_CreateWindow("Woopsi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT * 2, 0);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    SDL_RenderPresent(_renderer);

    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ABGR1555, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT * 2);

    _topBitmap = new u16[SCREEN_WIDTH * SCREEN_HEIGHT];
    _bottomBitmap = new u16[SCREEN_WIDTH * SCREEN_HEIGHT];

	_topBuffer = new FrameBuffer(_topBitmap, SCREEN_WIDTH, SCREEN_HEIGHT);
	_bottomBuffer = new FrameBuffer(_bottomBitmap, SCREEN_WIDTH, SCREEN_HEIGHT);

#endif

	_topGfx = _topBuffer->newGraphics();
	_bottomGfx = _bottomBuffer->newGraphics();
}

void Hardware::shutdown() {
	delete _topGfx;
	delete _bottomGfx;
	delete _topBuffer;
	delete _bottomBuffer;
	
#ifdef USING_SDL
	SDL_DestroyRenderer(_renderer);
    SDL_DestroyTexture(_texture);
    SDL_DestroyWindow(_window);
    
    delete _topBitmap;
    delete _bottomBitmap;
#endif
}

void Hardware::waitForVBlank() {

#ifndef USING_SDL

	swiWaitForVBlank();

#else

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = SCREEN_WIDTH;
    rect.h = SCREEN_HEIGHT;
	SDL_UpdateTexture(_texture, &rect, _topBitmap, SCREEN_WIDTH * sizeof(u16));

    rect.y = SCREEN_HEIGHT;
    SDL_UpdateTexture(_texture, &rect, _bottomBitmap, SCREEN_WIDTH * sizeof(u16));

    SDL_RenderCopy(_renderer, _texture, NULL, NULL);
    SDL_RenderPresent(_renderer);

	// SDL event pump
	SDL_Event event;
	
	// Check for SDL quit
	while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
				return;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == 53) {
                    // Escape pressed
					exit(0);
					return;
                }
                break;
        }
	}

#endif

	_pad.update();
	_stylus.update();
}
