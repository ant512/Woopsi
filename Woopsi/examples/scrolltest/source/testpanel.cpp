#include "testpanel.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

TestPanel::TestPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style) : ScrollingPanel(x, y, width, height, flags, style) {

	// Set the dimensions of the virtual canvas
	setCanvasHeight(300);
	setCanvasWidth(300);

	// Enable the virtual canvas to be dragged with the stylus
	_flags.draggable = true;
};
		
void TestPanel::draw(Rect clipRect) {

	// Create graphics port to draw to
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw background
	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	// Draw contents.  Note that all co-ordinates are offset by the canvas
	// x and y co-ordinates
	port->drawText(30 + getCanvasX(), 60 + getCanvasY(), getFont(), "Scrolling Panel Test", woopsiRGB(0, 0, 15));
	port->drawFilledRect(70 + getCanvasX(), 90 + getCanvasY(), 100, 100, woopsiRGB(15, 0, 0));

	// Clean up
	delete port;
};
