#include <string.h>
#include "screen.h"
#include "woopsi.h"
#include "woopsifuncs.h"

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

	if (isEnabled()) {
		if (checkCollision(x, y)) {

			// Work out which child was clicked
			for (s32 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					return true;
				}
			}

			// Handle clicks on this
			_flags.clicked = true;

			// Take focus away from child gadgets
			setFocusedGadget(NULL);

			// Tell parent that the clicked gadget has changed
			if (woopsiApplication != NULL) {
				woopsiApplication->setClickedGadget(this);
			}

			raiseClickEvent(x, y);

			return true;
		}
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

			// Calculate physical screen to use
			u8 screenNumber = calculatePhysicalScreenNumber(y);

			// Abort dragging if not dragging the bottom screen; will only be an issue in SDL code
#ifdef USING_SDL
			if (screenNumber != 0) return false;
#endif

			y = calculatePhysicalScreenY(y);

			// Work out where we're moving to
			s16 destY = y - _grabPointY;

			// Do we need to move?
			if (destY != _y) {

				if (destY < 0) {
					destY = 0;
				}

				// Perform move
				_newY = destY;

				// Ensure vY is valid
				vY = _newY - _y;

				if (_newY != _y) {

					// Create pointer to a vector to store the overlapped rectangles
					// We can discard this later as we don't need it
					WoopsiArray<Rect>* invisibleRects = new WoopsiArray<Rect>();

					// Create pointer to a vector to store the non-overlapped rectangles
					// We will use this to clip the gadget
					WoopsiArray<Rect>* visibleRects = new WoopsiArray<Rect>();

					// Copy the gadget's properties into a rect
					Rect vRect;
					vRect.x = getX();
					vRect.y = getY();
					vRect.width = getWidth();
					vRect.height = getHeight();

					// Clip to screen
					if (vRect.y + vRect.height > SCREEN_HEIGHT) {
						vRect.height = SCREEN_HEIGHT - vRect.y;
					}

					visibleRects->push_back(vRect);

					// Are we at the top level?
					if (_parent != NULL) {
						// Request refresh
						_parent->removeOverlappedRects(visibleRects, invisibleRects, this);
					}

					if (visibleRects->size() > 0) {

						// Use DMA_Copy to scroll all rows

						u16* srcLinei;
						u16 lineInc;
						u16* destLinei;

						// Precalculate values
						if (vY > 0) {
							// Scroll down
							srcLinei = DrawBg[screenNumber] + getX() + ((visibleRects->at(0).y + visibleRects->at(0).height - vY - 1) * SCREEN_WIDTH);
							lineInc = SCREEN_WIDTH;
							destLinei = srcLinei + (vY * lineInc);

							for (s32 i = 0; i < visibleRects->at(0).height - vY; i++) {
								while(DMA_Active());
								DMA_Copy(srcLinei, destLinei, _width, DMA_16NOW);
								

								// Move to next set of lines
								srcLinei -= lineInc;
								destLinei -= lineInc;
							}
						} else if (vY < 0) {
							// Scroll up
							srcLinei = DrawBg[screenNumber] + getX() + (visibleRects->at(0).y * SCREEN_WIDTH);
							lineInc = SCREEN_WIDTH;
							destLinei = srcLinei + (vY * lineInc);

							for (s32 i = 0; i < visibleRects->at(0).height; i++) {
								while(DMA_Active());
								DMA_Copy(srcLinei, destLinei, _width, DMA_16NOW);
								

								// Move to next set of lines
								srcLinei += lineInc;
								destLinei += lineInc;
							}
						}
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

						if (visibleRects->size() > 0) {

							// Screen is visible, so use the visible rectangle values
							rect.y = visibleRects->at(0).y + visibleRects->at(0).height + vY;
							rect.height = -(vY);

						} else {

							// Screen is not yet visible, to calculate based on screen values
							rect.y = _newY;
							rect.height = _y - _newY;
						}
					}

					// Tidy up
					delete visibleRects;
					delete invisibleRects;

					_y = _newY;

					// Erase the screen from its old location
					((Woopsi*)_parent)->eraseRect(rect);
					//_parent->draw(rect);

					raiseDragEvent(x, y, vX, vY);
				}
			}

			// Handle visible region caching
			invalidateVisibleRectCache();

			if (_parent != NULL) {
				_parent->invalidateLowerGadgetsVisibleRectCache(this);
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
			eraseGadget(gadget);

			// Swap
			_gadgets.erase(_gadgets.begin() + gadgetSource);
			_gadgets.insert(_gadgets.begin() + gadgetDest, gadget);

			// Invalidate all gadgets that collide with the shifted gadget
			for (s32 i = 0; i < _gadgets.size(); i++) {
				if (_gadgets[i]->checkCollision(gadget)) {
					_gadgets[i]->invalidateVisibleRectCache();
				}
			}

			// Redraw the gadget
			gadget->draw();

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

	draw();
}
