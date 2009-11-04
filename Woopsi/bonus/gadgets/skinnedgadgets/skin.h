#ifndef _SKIN_H_
#define _SKIN_H_

#include <nds.h>
#include "fontbase.h"

namespace WoopsiUI {

	typedef struct {
		const u16* focus;
		const u16* blur;
		const u16* focusClick;
		const u16* blurClick;
		u16 width;
		u16 height;
	} SkinBitmap;

	typedef struct {
		FontBase* font;
		u16 colour;
		bool monochrome;
	} SkinFont;

	typedef struct {
		u16 back;
		u16 shine;
		u16 highlight;
		u16 shadow;
		u16 fill;
	} SkinColours;

	typedef struct {
		SkinFont font;
		SkinColours colours;
		SkinBitmap bitmap;
		u16 offsetX;
		u16 offsetY;
		u8 permeable : 1;
		u8 borderless : 1;
		u8 visible : 1;
	} GadgetSkin;

	typedef struct {
		GadgetSkin titleBar;
		GadgetSkin depthButton;
		GadgetSkin flipButton;
		GadgetSkin screen;
	} ScreenSkin;

	typedef struct {
		GadgetSkin topLeftBorder;
		GadgetSkin topCentreBorder;
		GadgetSkin topRightBorder;
		GadgetSkin leftBorder;
		GadgetSkin rightBorder;
		GadgetSkin bottomLeftBorder;
		GadgetSkin bottomRightBorder;
		GadgetSkin bottomCentreBorder;
		GadgetSkin depthButton;
		GadgetSkin closeButton;
		GadgetSkin window;
	} WindowSkin;
}

#endif
