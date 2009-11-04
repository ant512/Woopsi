// Includes
#include "skins.h"
#include "skinnedscreen.h"
#include "skinnedwindow.h"
#include "textbox.h"
#include "woopsifuncs.h"
#include "../data/gfx.c"
#include "../data/gfx.h"

void Skins::startup() {

	// Call base startup method
	Woopsi::startup();

	// Create screen skin
	ScreenSkin* screenSkin = new ScreenSkin;
	screenSkin->screen.colours.back = woopsiRGB(8, 8, 8);
	screenSkin->screen.colours.shine = woopsiRGB(31, 31, 31);
	screenSkin->screen.colours.highlight = woopsiRGB(12, 17, 23);
	screenSkin->screen.colours.shadow = woopsiRGB(0, 0, 0);
	screenSkin->screen.colours.fill = woopsiRGB(24, 24, 24);
	screenSkin->screen.font.font = Woopsi::getSystemFont();
	screenSkin->screen.font.monochrome = true;
	screenSkin->screen.font.colour = woopsiRGB(0, 0, 0);
	screenSkin->screen.visible = true;
	screenSkin->screen.permeable = true;
	screenSkin->screen.borderless = true;

	screenSkin->titleBar.bitmap.focus = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.blur = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.focusClick = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.blurClick = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.width = 256;
	screenSkin->titleBar.bitmap.height = 13;
	screenSkin->titleBar.visible = true;
	screenSkin->titleBar.borderless = true;
	screenSkin->titleBar.offsetX = 0;
	screenSkin->titleBar.offsetY = 0;

	screenSkin->depthButton.bitmap.focus = screendepthnormal_Bitmap;
	screenSkin->depthButton.bitmap.blur = screendepthnormal_Bitmap;
	screenSkin->depthButton.bitmap.focusClick = screendepthclick_Bitmap;
	screenSkin->depthButton.bitmap.blurClick = screendepthclick_Bitmap;
	screenSkin->depthButton.bitmap.width = 12;
	screenSkin->depthButton.bitmap.height = 12;
	screenSkin->depthButton.visible = true;
	screenSkin->depthButton.borderless = true;
	screenSkin->depthButton.offsetX = 0;
	screenSkin->depthButton.offsetY = 0;

	screenSkin->flipButton.bitmap.focus = screenflipnormal_Bitmap;
	screenSkin->flipButton.bitmap.blur = screenflipnormal_Bitmap;
	screenSkin->flipButton.bitmap.focusClick = screenflipclick_Bitmap;
	screenSkin->flipButton.bitmap.blurClick = screenflipclick_Bitmap;
	screenSkin->flipButton.bitmap.width = 12;
	screenSkin->flipButton.bitmap.height = 12;
	screenSkin->flipButton.visible = true;
	screenSkin->flipButton.borderless = true;
	screenSkin->flipButton.offsetX = 0;
	screenSkin->flipButton.offsetY = 0;

	// Create window skin
	WindowSkin* windowSkin = new WindowSkin;
	windowSkin->window.colours.back = woopsiRGB(8, 8, 8);
	windowSkin->window.colours.shine = woopsiRGB(31, 31, 31);
	windowSkin->window.colours.highlight = woopsiRGB(12, 17, 23);
	windowSkin->window.colours.shadow = woopsiRGB(0, 0, 0);
	windowSkin->window.colours.fill = woopsiRGB(24, 24, 24);
	windowSkin->window.font.font = Woopsi::getSystemFont();
	windowSkin->window.font.monochrome = true;
	windowSkin->window.font.colour = woopsiRGB(0, 0, 0);
	windowSkin->window.visible = true;
	windowSkin->window.permeable = false;
	windowSkin->window.borderless = false;

	windowSkin->closeButton.bitmap.focus = winclosenormal_Bitmap;
	windowSkin->closeButton.bitmap.blur = winclosenormal_Bitmap;
	windowSkin->closeButton.bitmap.focusClick = wincloseclick_Bitmap;
	windowSkin->closeButton.bitmap.blurClick = wincloseclick_Bitmap;
	windowSkin->closeButton.bitmap.width = 9;
	windowSkin->closeButton.bitmap.height = 9;
	windowSkin->closeButton.visible = true;
	windowSkin->closeButton.borderless = true;
	windowSkin->closeButton.offsetX = 4;
	windowSkin->closeButton.offsetY = 0;

	windowSkin->depthButton.bitmap.focus = windepthnormal_Bitmap;
	windowSkin->depthButton.bitmap.blur = windepthnormal_Bitmap;
	windowSkin->depthButton.bitmap.focusClick = windepthclick_Bitmap;
	windowSkin->depthButton.bitmap.blurClick = windepthclick_Bitmap;
	windowSkin->depthButton.bitmap.width = 9;
	windowSkin->depthButton.bitmap.height = 9;
	windowSkin->depthButton.visible = true;
	windowSkin->depthButton.borderless = true;
	windowSkin->depthButton.offsetX = 4;
	windowSkin->depthButton.offsetY = 0;

	windowSkin->topLeftBorder.bitmap.focus = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.blur = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.focusClick = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.blurClick = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.width = 8;
	windowSkin->topLeftBorder.bitmap.height = 13;

	windowSkin->topCentreBorder.bitmap.focus = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.blur = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.focusClick = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.blurClick = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.width = 256;
	windowSkin->topCentreBorder.bitmap.height = 13;

	windowSkin->topRightBorder.bitmap.focus = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.blur = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.focusClick = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.blurClick = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.width = 8;
	windowSkin->topRightBorder.bitmap.height = 13;

	windowSkin->leftBorder.bitmap.focus = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.blur = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.focusClick = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.blurClick = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.width = 5;
	windowSkin->leftBorder.bitmap.height = 192;

	windowSkin->rightBorder.bitmap.focus = winright_Bitmap;
	windowSkin->rightBorder.bitmap.blur = winright_Bitmap;
	windowSkin->rightBorder.bitmap.focusClick = winright_Bitmap;
	windowSkin->rightBorder.bitmap.blurClick = winright_Bitmap;
	windowSkin->rightBorder.bitmap.width = 5;
	windowSkin->rightBorder.bitmap.height = 192;

	windowSkin->bottomLeftBorder.bitmap.focus = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.blur = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.focusClick = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.blurClick = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.width = 5;
	windowSkin->bottomLeftBorder.bitmap.height = 5;

	windowSkin->bottomCentreBorder.bitmap.focus = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.blur = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.focusClick = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.blurClick = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.width = 256;
	windowSkin->bottomCentreBorder.bitmap.height = 5;

	windowSkin->bottomRightBorder.bitmap.focus = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.blur = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.focusClick = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.blurClick = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.width = 5;
	windowSkin->bottomRightBorder.bitmap.height = 5;
	
	// Create skinned screen
	SkinnedScreen* sknScreen = new SkinnedScreen("Skin", Gadget::GADGET_DRAGGABLE, SkinnedScreen::SKINNED_SCREEN_SHOW_FLIP | SkinnedScreen::SKINNED_SCREEN_SHOW_DEPTH, screenSkin);
	woopsiApplication->addGadget(sknScreen);

	// Create skinned window
	SkinnedWindow* sknWindow = new SkinnedWindow(10, 10, 100, 100, "window", Gadget::GADGET_DRAGGABLE, SkinnedWindow::SKINNED_WINDOW_SHOW_CLOSE | SkinnedWindow::SKINNED_WINDOW_SHOW_DEPTH, windowSkin);
	sknScreen->addGadget(sknWindow);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void Skins::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
