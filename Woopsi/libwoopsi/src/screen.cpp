#include <string.h>
#include "screen.h"
#include "woopsi.h"
#include "woopsifuncs.h"
#include "rectcache.h"
#include "graphicsport.h"

using namespace WoopsiUI;

Screen::Screen(const char* title, u32 flags, FontBase* font) : Gadget(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, flags, font) {
	_titleHeight = 0;
	
	_title = NULL;
	setTitle(title);
	
	_flags.borderless = true;
}

bool Screen::focus() {
	
	if (isEnabled()) {
		if (!_flags.hasFocus) {
			_flags.hasFocus = true;
			
			raiseToTop();
			
			// Notify parent that this gadget has focus
			if (_parent != NULL) {
				_parent->setFocusedGadget(this);
			}
			
			raiseFocusEvent();
			
			return true;
		}
	}
	
	return false;
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

void Screen::draw(Rect clipRect) {
	clear(clipRect);
}

bool Screen::click(s16 x, s16 y) {
	
	if (checkCollision(x, y)) {
			
		// Work out which child was clicked
		for (s32 i = _gadgets.size() - 1; i > -1; i--) {
			if (_gadgets[i]->click(x, y)) {
				return true;
			}
		}

		if (isEnabled()) {
			
			// Handle clicks on this
			_flags.clicked = true;
			
			// Take focus away from child gadgets
			setFocusedGadget(NULL);
			
			// Tell parent that the clicked gadget has changed
			if (woopsiApplication != NULL) {
				woopsiApplication->setClickedGadget(this);
			}
			
			raiseClickEvent(x, y);
		}
			
		return true;
	}
	
	return false;
}

bool Screen::release(s16 x, s16 y) {
	
	// Release from dragging
	if (_flags.dragging) {
		_y = _newY;
		
		// Handle release on screen
		Gadget::release(x, y);
		
		return true;
	} else if (_flags.clicked) {
		// Handle release on screen
		Gadget::release(x, y);
		
		return true;
	}
	
	return false;
}

bool Screen::drag(s16 x, s16 y, s16 vX, s16 vY) {
	
	if (isEnabled()) {
		if (_flags.dragging) {
			
#ifdef USING_SDL
			// Abort dragging if not dragging the bottom screen; will only be an issue in SDL code
			if (calculatePhysicalScreenNumber(y) != 0) return false;
#endif
			
			y = calculatePhysicalScreenY(y);
			
			// Work out where we're moving to
			s16 destY = y - _grabPointY;
			
			// Prevent screen moving too far up
			if (destY < 0) {
				destY = 0;
			}
			
			// Do we need to move?
			if (destY != _y) {
				
				// Perform move
				_newY = destY;
				
				// Ensure vY is valid
				vY = _newY - _y;
				
				if (_newY != _y) {
					
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
						if (_newY > _y) rect.height -= _newY - _y;
						
						GraphicsPort* port = newGraphicsPort(true);
						port->copy(rect.x, rect.y, rect.x, rect.y + vY, rect.width, rect.height);
						
						delete port;
					}
					
					// Work out the size of the rectangle we've cleared
					Rect rect;
					rect.x = getX();
					rect.width = _width;
					
					if (_newY > _y) {
						
						// Moving down - we need to redraw the section we're
						// exposing
						rect.y = _y;
						rect.height = _newY - _y;
						
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
							rect.height = _y - _newY;
						}
					}
					
					_y = _newY;
					
					// Erase the screen from its old location
					((Woopsi*)_parent)->eraseRect(rect);
					
					raiseDragEvent(x, y, vX, vY);
					
					// Handle visible region caching
					invalidateVisibleRectCache();
					
					if (_parent != NULL) {
						_parent->invalidateLowerGadgetsVisibleRectCache(this);
					}
				}
			}
			
			return true;
		}
	}
	
	return false;
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
			eraseGadget(gadget);
			
			// Swap
			_gadgets.erase(gadgetSource);
			_gadgets.insert(gadgetDest, gadget);
			
			// Invalidate all gadgets that collide with the shifted gadget
			for (s32 i = 0; i < _gadgets.size(); i++) {
				if (_gadgets[i]->checkCollision(gadget)) {
					_gadgets[i]->invalidateVisibleRectCache();
				}
			}
			
			// Redraw the gadget
			gadget->redraw();
			
			// Give focus to the highest gadget in the stack
			_gadgets[_gadgets.size() - 1]->focus();
			
			return true;
		}
	}
	
	return false;
}

// Insert the available space for child gadgets into the rect
void Screen::getClientRect(Rect& rect) const {
	rect.x = 0;
	rect.y = 0;
	rect.width = _width;
	rect.height = _height;
}

void Screen::setTitle(const char* title) {

	// Have we already created a block of memory that we need to free?
	if (_title != NULL) {
		// Free the memory
		delete [] _title;
	}

	// Create new memory for string
	_title = new char[strlen(title) + 1];

	// Copy text
	strcpy(_title, title);

	redraw();
}
