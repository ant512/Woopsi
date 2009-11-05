#include "skinio.h"
#include "bitmap.h"
#include "bitmapio.h"
#include "debug.h"

using namespace WoopsiUI;

void SkinIO::loadScreenSkin(ScreenSkin& skin, const char* path) {

	// Ensure skin is set to default values
	resetGadgetSkin(skin.titleBar);
	resetGadgetSkin(skin.depthButton);
	resetGadgetSkin(skin.flipButton);
	resetGadgetSkin(skin.screen);

	// Load bitmaps from filesystem
	
	// Title bar
	skin.titleBar.bitmap.focus = loadBMP("screentitle.bmp", path);
	skin.titleBar.bitmap.blur = skin.titleBar.bitmap.focus;
	skin.titleBar.bitmap.focusClick = skin.titleBar.bitmap.focus;
	skin.titleBar.bitmap.blurClick = skin.titleBar.bitmap.focus;
	skin.titleBar.bitmap.width = skin.titleBar.bitmap.focus->getWidth();
	skin.titleBar.bitmap.height = skin.titleBar.bitmap.focus->getHeight();
	
	// Depth button
	skin.depthButton.bitmap.focus = loadBMP("screendepthnormal.bmp", path);
	skin.depthButton.bitmap.blur = skin.depthButton.bitmap.focus;
	skin.depthButton.bitmap.focusClick = loadBMP("screendepthclick.bmp", path);
	skin.depthButton.bitmap.blurClick = skin.depthButton.bitmap.focusClick;
	skin.depthButton.bitmap.width = skin.depthButton.bitmap.focus->getWidth();
	skin.depthButton.bitmap.height = skin.depthButton.bitmap.focus->getHeight();
	
	// Flip button
	skin.flipButton.bitmap.focus = loadBMP("screenflipnormal.bmp", path);
	skin.flipButton.bitmap.blur = skin.flipButton.bitmap.focus;
	skin.flipButton.bitmap.focusClick = loadBMP("screenflipclick.bmp", path);
	skin.flipButton.bitmap.blurClick = skin.flipButton.bitmap.focusClick;
	skin.flipButton.bitmap.width = skin.flipButton.bitmap.focus->getWidth();
	skin.flipButton.bitmap.height = skin.flipButton.bitmap.focus->getHeight();
}

void SkinIO::loadWindowSkin(WindowSkin& skin, const char* path) {

	// Ensure skin is set to default values
	resetGadgetSkin(skin.topLeftBorder);
	resetGadgetSkin(skin.topCentreBorder);
	resetGadgetSkin(skin.topRightBorder);
	resetGadgetSkin(skin.leftBorder);
	resetGadgetSkin(skin.rightBorder);
	resetGadgetSkin(skin.bottomLeftBorder);
	resetGadgetSkin(skin.bottomRightBorder);
	resetGadgetSkin(skin.bottomCentreBorder);
	resetGadgetSkin(skin.depthButton);
	resetGadgetSkin(skin.closeButton);
	resetGadgetSkin(skin.window);
	

	// Load bitmaps from filesystem
	
	// Top left border
	skin.topLeftBorder.bitmap.focus = loadBMP("wintitleleftfocus.bmp", path);
	skin.topLeftBorder.bitmap.blur = loadBMP("wintitleleftblur.bmp", path);
	skin.topLeftBorder.bitmap.focusClick = skin.topLeftBorder.bitmap.focus;
	skin.topLeftBorder.bitmap.blurClick = skin.topLeftBorder.bitmap.blur;
	skin.topLeftBorder.bitmap.width = skin.topLeftBorder.bitmap.focus->getWidth();
	skin.topLeftBorder.bitmap.height = skin.topLeftBorder.bitmap.focus->getHeight();
	
	// Top centre border
	skin.topCentreBorder.bitmap.focus = loadBMP("wintitlecentrefocus.bmp", path);
	skin.topCentreBorder.bitmap.blur = loadBMP("wintitlecentreblur.bmp", path);
	skin.topCentreBorder.bitmap.focusClick = skin.topCentreBorder.bitmap.focus;
	skin.topCentreBorder.bitmap.blurClick = skin.topCentreBorder.bitmap.blur;
	skin.topCentreBorder.bitmap.width = skin.topCentreBorder.bitmap.focus->getWidth();
	skin.topCentreBorder.bitmap.height = skin.topCentreBorder.bitmap.focus->getHeight();
	
	// Top right border
	skin.topRightBorder.bitmap.focus = loadBMP("wintitlerightfocus.bmp", path);
	skin.topRightBorder.bitmap.blur = loadBMP("wintitlerightblur.bmp", path);
	skin.topRightBorder.bitmap.focusClick = skin.topRightBorder.bitmap.focus;
	skin.topRightBorder.bitmap.blurClick = skin.topRightBorder.bitmap.blur;
	skin.topRightBorder.bitmap.width = skin.topRightBorder.bitmap.focus->getWidth();
	skin.topRightBorder.bitmap.height = skin.topRightBorder.bitmap.focus->getHeight();
	
	// Left border
	skin.leftBorder.bitmap.focus = loadBMP("winleftfocus.bmp", path);
	skin.leftBorder.bitmap.blur = loadBMP("winleftblur.bmp", path);
	skin.leftBorder.bitmap.focusClick = skin.leftBorder.bitmap.focus;
	skin.leftBorder.bitmap.blurClick = skin.leftBorder.bitmap.blur;
	skin.leftBorder.bitmap.width = skin.leftBorder.bitmap.focus->getWidth();
	skin.leftBorder.bitmap.height = skin.leftBorder.bitmap.focus->getHeight();
	
	// Right border
	skin.rightBorder.bitmap.focus = loadBMP("winrightfocus.bmp", path);
	skin.rightBorder.bitmap.blur = loadBMP("winrightblur.bmp", path);
	skin.rightBorder.bitmap.focusClick = skin.rightBorder.bitmap.focus;
	skin.rightBorder.bitmap.blurClick = skin.rightBorder.bitmap.blur;
	skin.rightBorder.bitmap.width = skin.rightBorder.bitmap.focus->getWidth();
	skin.rightBorder.bitmap.height = skin.rightBorder.bitmap.focus->getHeight();
	
	// Bottom left border
	skin.bottomLeftBorder.bitmap.focus = loadBMP("winbottomleftfocus.bmp", path);
	skin.bottomLeftBorder.bitmap.blur = loadBMP("winbottomleftblur.bmp", path);
	skin.bottomLeftBorder.bitmap.focusClick = skin.bottomLeftBorder.bitmap.focus;
	skin.bottomLeftBorder.bitmap.blurClick = skin.bottomLeftBorder.bitmap.blur;
	skin.bottomLeftBorder.bitmap.width = skin.bottomLeftBorder.bitmap.focus->getWidth();
	skin.bottomLeftBorder.bitmap.height = skin.bottomLeftBorder.bitmap.focus->getHeight();
	
	// Bottom centre border
	skin.bottomCentreBorder.bitmap.focus = loadBMP("winbottomcentrefocus.bmp", path);
	skin.bottomCentreBorder.bitmap.blur = loadBMP("winbottomcentreblur.bmp", path);
	skin.bottomCentreBorder.bitmap.focusClick = skin.bottomCentreBorder.bitmap.focus;
	skin.bottomCentreBorder.bitmap.blurClick = skin.bottomCentreBorder.bitmap.blur;
	skin.bottomCentreBorder.bitmap.width = skin.bottomCentreBorder.bitmap.focus->getWidth();
	skin.bottomCentreBorder.bitmap.height = skin.bottomCentreBorder.bitmap.focus->getHeight();
	
	// Bottom right border
	skin.bottomRightBorder.bitmap.focus = loadBMP("winbottomrightfocus.bmp", path);
	skin.bottomRightBorder.bitmap.blur = loadBMP("winbottomrightblur.bmp", path);
	skin.bottomRightBorder.bitmap.focusClick = skin.bottomRightBorder.bitmap.focus;
	skin.bottomRightBorder.bitmap.blurClick = skin.bottomRightBorder.bitmap.blur;
	skin.bottomRightBorder.bitmap.width = skin.bottomRightBorder.bitmap.focus->getWidth();
	skin.bottomRightBorder.bitmap.height = skin.bottomRightBorder.bitmap.focus->getHeight();
	
	// Depth button
	skin.depthButton.bitmap.focus = loadBMP("windepthnormalfocus.bmp", path);
	skin.depthButton.bitmap.blur = loadBMP("windepthnormalblur.bmp", path);
	skin.depthButton.bitmap.focusClick = loadBMP("windepthclickfocus.bmp", path);
	skin.depthButton.bitmap.blurClick = loadBMP("windepthclickblur.bmp", path);
	skin.depthButton.bitmap.width = skin.depthButton.bitmap.focus->getWidth();
	skin.depthButton.bitmap.height = skin.depthButton.bitmap.focus->getHeight();
	
	// Close button
	skin.closeButton.bitmap.focus = loadBMP("winclosenormalfocus.bmp", path);
	skin.closeButton.bitmap.blur = loadBMP("winclosenormalblur.bmp", path);
	skin.closeButton.bitmap.focusClick = loadBMP("wincloseclickfocus.bmp", path);
	skin.closeButton.bitmap.blurClick = loadBMP("wincloseclickblur.bmp", path);
	skin.closeButton.bitmap.width = skin.closeButton.bitmap.focus->getWidth();
	skin.closeButton.bitmap.height = skin.closeButton.bitmap.focus->getHeight();
}

Bitmap* SkinIO::loadBMP(const char* fileName, const char* path) {

	// Get lengths of filename and path as they will be used repeatedly
	u16 pathLength = strlen(path);
	u16 fileNameLength = strlen(fileName);
	
	// Create an array to store the full path and filename
	char* fullName = new char[pathLength + fileNameLength + 2];
	
	// Concatenate path and filename
	strcpy(fullName, path);
	strcpy(fullName + pathLength, "/");
	strcpy(fullName + pathLength + 1, fileName);
	
	Debug::printf(fullName);
	
	// Load the bitmap
	return BitmapIO::loadBMP(fullName);
}

void SkinIO::resetGadgetSkin(GadgetSkin& skin) {
	skin.font.font = NULL;
	skin.font.colour = 0;
	skin.font.monochrome = false;
	skin.colours.back = 0;
	skin.colours.shine = 0;
	skin.colours.highlight = 0;
	skin.colours.shadow = 0;
	skin.colours.fill = 0;
	skin.offsetX = 0;
	skin.offsetY = 0;
	skin.permeable = false;
	skin.borderless = false;
	skin.visible = true;
	skin.bitmap.focus = NULL;
	skin.bitmap.blur = NULL;
	skin.bitmap.focusClick = NULL;
	skin.bitmap.blurClick = NULL;
	skin.bitmap.width = 0;
	skin.bitmap.height = 0;
}
