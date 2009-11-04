// Includes
#include "skins.h"
#include "skinnedscreen.h"
#include "skinnedwindow.h"
#include "textbox.h"
#include "woopsifuncs.h"
#include "skinio.h"

void Skins::startup() {

	// Call base startup method
	Woopsi::startup();
	
	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
	
	fatInitDefault();

	// Create screen skin
	ScreenSkin* screenSkin = new ScreenSkin;
	SkinIO::loadScreenSkin(*screenSkin, "/skins/default");

	//ScreenSkin* screenSkin = new ScreenSkin;
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

	screenSkin->titleBar.visible = true;
	screenSkin->titleBar.borderless = true;
	screenSkin->titleBar.offsetX = 0;
	screenSkin->titleBar.offsetY = 0;

	screenSkin->depthButton.visible = true;
	screenSkin->depthButton.borderless = true;
	screenSkin->depthButton.offsetX = 0;
	screenSkin->depthButton.offsetY = 0;
	
	screenSkin->flipButton.visible = true;
	screenSkin->flipButton.borderless = true;
	screenSkin->flipButton.offsetX = 0;
	screenSkin->flipButton.offsetY = 0;

	// Create window skin
	WindowSkin* windowSkin = new WindowSkin;
	SkinIO::loadWindowSkin(*windowSkin, "/skins/default");
	
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

	windowSkin->closeButton.visible = true;
	windowSkin->closeButton.borderless = true;
	windowSkin->closeButton.offsetX = 4;
	windowSkin->closeButton.offsetY = 0;

	windowSkin->depthButton.visible = true;
	windowSkin->depthButton.borderless = true;
	windowSkin->depthButton.offsetX = 4;
	windowSkin->depthButton.offsetY = 0;
	
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
