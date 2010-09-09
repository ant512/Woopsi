#include <string.h>
#include "screen.h"
#include "woopsi.h"
#include "woopsifuncs.h"
#include "rectcache.h"
#include "graphicsport.h"
#include "damagedrectmanager.h"

using namespace WoopsiUI;

Screen::Screen(const WoopsiString& title, u32 flags, GadgetStyle* style) : Gadget(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, flags, style) {
	_titleHeight = 0;
	_title = title;
	_flags.borderless = true;

	_borderSize.top = 0;
	_borderSize.right = 0;
	_borderSize.bottom = 0;
	_borderSize.left = 0;
}

void Screen::onFocus() {
	raiseToTop();
}

void Screen::flipToTopScreen() {
	moveTo(0, TOP_SCREEN_Y_OFFSET);
}

void Screen::flipToBottomScreen() {
	moveTo(0, 0);
}

bool Screen::flipScreens() {
	if (_parent != NULL) {
		return ((Woopsi*)_parent)->flipScreens(this);
	}
	
	return false;
}

void Screen::drawBorder(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());
}

void Screen::onDrag(s16 x, s16 y, s16 vX, s16 vY) {
			
	y = calculatePhysicalScreenY(y);
	
	// Work out where we're moving to
	s16 destY = y - _grabPointY;
	
	// Prevent screen moving too far up
	if (destY < 0) {
		destY = 0;
	}
		
	// Do we need to move?
	if (destY != _rect.getY()) {
		
		// Perform move
		_newY = destY;
		
		// Ensure vY is valid
		vY = _newY - _rect.getY();
		
		if (_newY != _rect.getY()) {
			
			// Ensure cache is up to date before copying
			_rectCache->cache();
			
			// Copy the current screen display to its new location.  Only copy
			// a single rect as screens have at most one rect visible
			if (getRectCache()->getForegroundRegions()->size() == 1) {
				
				// Get dimensions of visible portion of screen
				Rect rect = getRectCache()->getForegroundRegions()->at(0);
				
				// Clip to display
				if (rect.y + rect.height > SCREEN_HEIGHT) {
					rect.height = SCREEN_HEIGHT - rect.y;
				}
				
				// Calculate height to copy - this is reduced if screen dragged down
				if (_newY > _rect.getY()) rect.height -= _newY - _rect.getY();
				
				if (rect.height > 0) {
					GraphicsPort* port = woopsiApplication->newGraphicsPort(true);
					port->copy(rect.x, rect.y, rect.x, rect.y + vY, rect.width, rect.height);
					delete port;
				}
			}
			
			// Work out the size of the rectangle we've cleared
			Rect rect;
			rect.x = getX();
			rect.width = getWidth();
			
			if (_newY > _rect.getY()) {
				
				// Moving down - we need to redraw the section we're
				// exposing
				rect.y = _rect.getY();
				rect.height = _newY - _rect.getY();
				
			} else {
				// Moving up - we need to redraw the new section at
				// the bottom of the screen
				
				if (getRectCache()->getForegroundRegions()->size() > 0) {
					
					// Screen is visible - get data from visible rect
					rect.y = getRectCache()->getForegroundRegions()->at(0).y;
					rect.height = getRectCache()->getForegroundRegions()->at(0).height;
					
					// Clip to display
					if (rect.y + rect.height > SCREEN_HEIGHT) {
						rect.height = SCREEN_HEIGHT - rect.y;
					}
					
					// Adjust so we only reference the newly-exposed area
					rect.y += rect.height + vY;
					rect.height = -(vY);
					
				} else {
					
					// Screen is not yet visible, so calculate based on screen values
					rect.y = _newY;
					rect.height = _rect.getY() - _newY;
				}
			}
			
			_rect.setY(_newY);
			
			// Erase the screen from its old location
			woopsiApplication->getDamagedRectManager()->addDamagedRect(rect);
			
			// Handle visible region caching
			invalidateVisibleRectCache();
			
			if (_parent != NULL) {
				_parent->invalidateLowerGadgetsVisibleRectCache(this);
			}
		}
	}
}

// Only allows non-decoration depths to be swapped
bool Screen::swapGadgetDepth(Gadget* gadget) {
	
	// Do we have more than one gadget?
	if (_gadgets.size() - _decorationCount > 1) {
		
		s32 lowestGadget = _decorationCount;
		s32 gadgetSource = getGadgetIndex(gadget);
		s32 gadgetDest = _decorationCount;
		s32 highestCollisionDepth = 0;
		s32 lowestCollisionDepth = 0;
		
		// Calculate the positions of the highest and lowest gadgets
		// that the window collides with
		for (s32 i = _gadgets.size() - 1; i >= lowestGadget; i--) {
			if (_gadgets[i]->checkCollision(gadget)) {
				if (gadget != _gadgets[i]) {
					// Set highest gadget depth
					if (highestCollisionDepth == 0) {
						highestCollisionDepth = i;
					}
					
					// Set lowest gadget depth
					lowestCollisionDepth = i;
				}
			}
		}
		
		// Work out where we're moving to
		if (highestCollisionDepth > gadgetSource) {
			// Moving up
			gadgetDest = highestCollisionDepth;
		} else if (lowestCollisionDepth > 0) {
			// Moving down
			gadgetDest = lowestCollisionDepth;
		}
		
		// Can we swap?
		if (gadgetDest != 0) {
			
			// Erase the gadget from the screen
			//gadget->cacheVisibleRects();
			gadget->markRectsDamaged();
			
			// Swap
			_gadgets.erase(gadgetSource);
			_gadgets.insert(gadgetDest, gadget);
			
			// Invalidate all gadgets that collide with the shifted gadget
			for (s32 i = 0; i < _gadgets.size(); i++) {
				if (_gadgets[i]->checkCollision(gadget)) {
					_gadgets[i]->invalidateVisibleRectCache();
				}
			}
			
			// Give focus to the highest gadget in the stack
			_gadgets[_gadgets.size() - 1]->focus();
			
			return true;
		}
	}
	
	return false;
}

void Screen::setTitle(const WoopsiString& title) {
	_title = title;

	markRectsDamaged();
}
