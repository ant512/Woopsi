/* Contributed by Steven
 * 20071213
 * - Minor modifications by Ant
 */

#include <nds.h>
#include <string.h>
#include "woopsifuncs.h"

u16* DrawBg[2];

#ifndef USING_PALIB

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
