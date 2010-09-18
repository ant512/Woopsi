#include "graphicsport.h"
#include "defines.h"
#include "fontbase.h"
#include "woopsifuncs.h"
#include "framebuffer.h"
#include "bitmapbase.h"
#include "stringiterator.h"

using namespace WoopsiUI;

GraphicsPort::GraphicsPort(const s16 x, const s16 y, const u16 width, const u16 height, const bool isEnabled, FrameBuffer* bitmap, const WoopsiArray<Rect>* clipRectList, const Rect* clipRect) {
	_rect.x = x;
	_rect.y = y;
	_rect.width = width;
	_rect.height = height;
	_isEnabled = isEnabled;
	
	_isTopScreen = (y >= TOP_SCREEN_Y_OFFSET) && (SCREEN_COUNT > 1);
	
	if (_isTopScreen) {
		int j = 0;
		j++;
	}

	_graphics = bitmap->newGraphics();
	
	// Set up clip rect
	if (clipRect != NULL) {
		setClipRect(*clipRect);
	} else {

		// Set up clip rect list
		if (clipRectList != NULL) {
			for (s32 i = 0; i < clipRectList->size(); ++i) {
				addClipRect(clipRectList->at(i));
			}
		}
	}
}

void GraphicsPort::addClipRect(const Rect& clipRect) {

	// Clip rect is clipped to the dimensions of the
	// GraphicsPort before it is stored.  This means
	// we eliminate a lot of complexity when drawing - 
	// we clip to rects, which are guaranteed to be
	// accurate - rather than trying to clip to both
	// the rects and the dimensions of the port.  It
	// also means we can ignore any cliprects that get
	// totally clipped out before they even get into
	// the port.
	Rect rect(clipRect);
	rect.clipToIntersect(_rect);

	// Do not add the rect if it has no size; we don't need to try and draw
	// empty rects
	if (!rect.hasDimensions()) return;
	
	_clipRectList.push_back(rect);
}

void GraphicsPort::setClipRect(const Rect& clipRect) {
	_clipRectList.clear();
	addClipRect(clipRect);
}

void GraphicsPort::getClipRect(Rect& rect) const {

	// The rect is adjusted such that its co-ordinates are relative to the
	// GraphicsPort before it is returned.  This makes using the rect
	// to optimise drawing easier.
	rect.x = _clipRectList[0].x - getX();
	rect.y = _clipRectList[0].y - getY();
	rect.width = _clipRectList[0].width;
	rect.height = _clipRectList[0].height;
}

// Print a string in a specific colour
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawText(x, y, font, string, startIndex, length);
	}
}

// Print a string in a specific colour on a baseline
void GraphicsPort::drawBaselineText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawBaselineText(x, y, font, string, startIndex, length);
	}
}

void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string) {
	drawText(x, y, font, string, 0, string.getLength());
}

// Print a string in a specific colour
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;

	// Store current font colour
	bool isMonochrome = font->isMonochrome();
	u16 oldColour = font->getColour();
	
	// Update font colour
	font->setColour(colour);
	
	// Output as normal
	drawText(x, y, font, string, startIndex, length);
	
	// Reset colour
	if (!isMonochrome) {
		font->clearColour();
	} else {
		font->setColour(oldColour);
	}
}

// Draw filled rectangle - external function
void GraphicsPort::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawFilledRect(x, y, width, height, colour);
	}
}

void GraphicsPort::drawEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&xCentre, &yCentre);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawEllipse(xCentre, yCentre, horizRadius, vertRadius, colour);
	}
}


void GraphicsPort::drawFilledEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&xCentre, &yCentre);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawFilledEllipse(xCentre, yCentre, horizRadius, vertRadius, colour);
	}
}

void GraphicsPort::drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawRect(x, y, width, height, colour);
	}
}

void GraphicsPort::drawBevelledRect(s16 x, s16 y, u16 width, u16 height, u16 shineColour, u16 shadowColour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawBevelledRect(x, y, width, height, shineColour, shadowColour);
	}
}

void GraphicsPort::drawFilledXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawFilledXORRect(x, y, width, height, colour);
	}
}

void GraphicsPort::drawFilledXORRect(s16 x, s16 y, u16 width, u16 height) {
	drawFilledXORRect(x, y, width, height, 0xffff);
}

void GraphicsPort::drawXORRect(s16 x, s16 y, u16 width, u16 height) {
	drawXORRect(x, y, width, height, 0xffff);
}

void GraphicsPort::drawXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawXORRect(x, y, width, height, colour);
	}
}

//Draw bitmap - external function
void GraphicsPort::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawBitmap(x, y, width, height, bitmap, bitmapX, bitmapY);
	}
}

//Draw bitmap with transparency - external function
void GraphicsPort::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY, u16 transparentColour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, transparentColour);
	}
}

void GraphicsPort::drawBitmapGreyScale(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawBitmapGreyScale(x, y, width, height, bitmap, bitmapX, bitmapY);
	}
}

void GraphicsPort::drawXORHorizLine(s16 x, s16 y, u16 width) {
	drawXORHorizLine(x, y, width, 0xffff);
}

void GraphicsPort::drawXORHorizLine(s16 x, s16 y, u16 width, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawXORHorizLine(x, y, width, colour);
	}
}

void GraphicsPort::drawXORVertLine(s16 x, s16 y, u16 height, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawXORVertLine(x, y, height, colour);
	}
}

void GraphicsPort::drawXORVertLine(s16 x, s16 y, u16 height) {
	drawXORVertLine(x, y, height, 0xffff);
}

// Adjust co-ords from port-space to screen-space
void GraphicsPort::convertPortToScreenSpace(s16* x, s16* y) {
	*x += getX();
	*y += getY();

	// Compensate for top screen offset
	if (_isTopScreen) *y -= TOP_SCREEN_Y_OFFSET;
}

void GraphicsPort::drawPixel(s16 x, s16 y, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);

		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawPixel(x, y, colour);
	}
}

void GraphicsPort::drawXORPixel(s16 x, s16 y) {
	drawXORPixel(x, y, 0xffff);
}

void GraphicsPort::drawXORPixel(s16 x, s16 y, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawXORPixel(x, y, colour);
	}
}

void GraphicsPort::drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x1, &y1);
	convertPortToScreenSpace(&x2, &y2);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->drawLine(x1, y1, x2, y2, colour);
	}
}

void GraphicsPort::copy(s16 sourceX, s16 sourceY, s16 destX, s16 destY, u16 width, u16 height) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&sourceX, &sourceY);
	convertPortToScreenSpace(&destX, &destY);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->copy(sourceX, sourceY, destX, destY, width, height);
	}
}

void GraphicsPort::scroll(s16 x, s16 y, s16 xDistance, s16 yDistance, u16 width, u16 height, WoopsiArray<Rect>* revealedRects) {
	
	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->scroll(x, y, xDistance, yDistance, width, height, revealedRects);
	}
	
	// Adjust revealedRects back to graphicsport space
	for (s32 i = 0; i < revealedRects->size(); ++i) {
		revealedRects->at(i).x -= _rect.x;
		revealedRects->at(i).y -= _rect.y;
		
		if (_isTopScreen) revealedRects->at(i).y += TOP_SCREEN_Y_OFFSET;
	}
}

void GraphicsPort::dim(s16 x, s16 y, u16 width, u16 height) {

	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->dim(x, y, width, height);
	}
}

void GraphicsPort::greyScale(s16 x, s16 y, u16 width, u16 height) {

	// Ignore command if drawing is disabled
	if (!_isEnabled) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);

	Rect rect;
	
	// Draw all visible rects
	for (s32 i = 0; i < _clipRectList.size(); i++) {
		
		// Adjust from graphicsport co-ordinates to framebuffer co-ordinates
		_clipRectList.at(i).copyTo(rect);
		
		if (_isTopScreen) rect.y -= TOP_SCREEN_Y_OFFSET;
		
		_graphics->setClipRect(rect);
		_graphics->greyScale(x, y, width, height);
	}
}
