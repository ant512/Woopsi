#ifndef _GADGET_H_
#define _GADGET_H_

#include <nds.h>
#include "eventhandler.h"
#include "debug.h"
#include "glyphs.h"
#include "defines.h"
#include "fontbase.h"
#include "dynamicarray.h"

using namespace std;

class GraphicsPort;

/**
 * Class proving all the basic functionality of a Woopsi gadget.
 * All Woopsi gadgets must inherit from this class.
 */
class Gadget {
public:

	/**
	 * Enum describing the types of border around a gadget.
	 */
	enum OutlineType {
		OUTLINE_CLICK_DEPENDENT = 0,	/**< Border depends on current click status */
		OUTLINE_OUT = 1,				/**< Border is bevelled "out" */
		OUTLINE_IN = 2					/**< Border is bevelled *in* */
	};

	/**
	 * Enum describing the way other gadgets should behave when they try to close this gadget.
	 */
	enum CloseType {
		CLOSE_TYPE_CLOSE = 0,			/**< Gadgets should call the close() method */
		CLOSE_TYPE_HIDE = 1				/**< Gadgets should call the hide() method */
	};

	/**
	 * Enum listing flags that can be set in the constructor's "flags" parameter.
	 */
	enum GadgetFlagType {
		GADGET_BORDERLESS = 0x0001,		/**< Gadget has no border */
		GADGET_CLOSEABLE = 0x0002,		/**< Gadget can be closed by the user */
		GADGET_DRAGGABLE = 0x0004,		/**< Gadget can be dragged by the user */
		GADGET_PERMEABLE = 0x0008		/**< Gadget's children can exceed this gadget's edges */
	};

	/**
	 * Struct describing a rectangle.
	 */
	typedef struct {
		s16 x;
		s16 y;
		s32 width;
		s32 height;
	} Rect;

	/**
	 * Struct describing some basic properties of a gadget.
	 */
	typedef struct {
		u8 clicked : 1;
		u8 active : 1;
		u8 dragging : 1;
		u8 deleted : 1;
		u8 borderless : 1;
		u8 draggable : 1;
		u8 visible : 1;
		u8 closeable : 1;
		u8 enabled : 1;
		u8 decoration : 1;
		u8 permeable : 1;
		u8 firesEvents : 1;
	} Flags;

	Gadget(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font = NULL);
	virtual ~Gadget();

	/**
	 * Get the x co-ordinate of the gadget relative to its parent.
	 * @return Parent-relative x co-ordinate.
	 */
	s16 getX() const;

	/**
	 * Get the y co-ordinate of the gadget relative to its parent.
	 * @return Parent-relative y co-ordinate.
	 */
	s16 getY() const;

	/**
	 * Get the reference constant for this gadget.
	 * @return The reference constant.
	 */
	u32 getRefcon();

	/**
	 * Get the type of close routine that should be called by other gadgets
	 * interacting with this gadget.
	 *
	 * @return The close type of this gadget.
	 */
	CloseType getCloseType();

	/**
	 * Is the gadget active?
	 * A value of true indicates that this gadget has focus or is an ancestor
	 * of the gadget with focus.
	 *
	 * @return True if active.
	 */
	const bool isActive() const;

	/**
	 * Has the gadget been marked for deletion?
	 * Gadgets marked for deletion are automatically deleted and should not be
	 * interacted with.
	 *
	 * @return True if marked for deletion.
	 */
	const bool isDeleted() const;

	/**
	 * Is the gadget visible?
	 * @return True if visible.
	 */
	const bool isVisible() const;

	/**
	 * Is the gadget a decoration?
	 * Decoration gadgets are children of, but also an essential component of,
	 * another gadget.
	 *
	 * @return True if the gadget is a decoration.
	 */
	const bool isDecoration() const;

	/**
	 * Are the gadget's edges permeable or solid?
	 * Permeable gadgets do not enforce their dimensions on the
	 * co-ordinates and dimensions of child gadgets.
	 *
	 * @return True if permeable.
	 */
	const bool isPermeable() const;

	/**
	 * Does the gadget have a border?
	 * @return True if the gadget does not have a border.
	 */
	const bool isBorderless() const;

	/**
	 * Is the gadget clicked?
	 * @return True if the gadget is currently clicked.
	 */
	const bool isClicked() const;

	/**
	 * Get the width of the gadget.
	 * @return The gadget width.
	 */
	u16 getWidth() const;

	/**
	 * Get the height of the gadget.
	 * @return The gadget height.
	 */
	u16 getHeight() const;

	/**
	 * Get a pointer to this gadget's parent.
	 * @return This gadget's parent.
	 */
	Gadget* getParent();

	/**
	 * Get a pointer to this gadget's active child.
	 * @return This gadget's active child.
	 */
	Gadget* getActiveGadget();

	/**
	 * Get the number of the screen that this gadget is currently displayed on.
	 * @return 0 for the touch screen, 1 for the top screen.
	 */
	const u8 getPhysicalScreenNumber() const;

	/**
	 * Check if this gadget fires events or not.
	 * @return True if events are enabled.
	 */
	const bool firesEvents() const;

	/**
	 * Insert the properties of the space within this gadget that is available
	 * for children into the rect passed in as a parameter.
	 * All co-ordinates are relative to this gadget.
	 *
	 * @param rect Reference to a rect to populate with data.
	 */
	virtual void getClientRect(Rect& rect) const;

	/**
	 * Clips the supplied rect to the boundaries defined by this gadget and
	 * this gadget's parents.
	 *
	 * @param rect Reference to a rect to populate with data.
	 */
	void getRectClippedToHierarchy(Rect& rect);

	/**
	 * Gets a pointer to a new instance of the GraphicsPort class to allow
	 * drawing within this gadget's client space.
	 * The GraphicsPort object must be deleted when it is no longer required.
	 *
	 * @return A pointer to a new GraphicsPort object.
	 */
	virtual GraphicsPort* newGraphicsPort();

	/**
	 * Gets a pointer to a new instance of the GraphicsPort class to allow
	 * drawing within this gadget's client space.  The GraphicsPort can
	 * only draw within the supplied region.
	 *
	 * The GraphicsPort object must be deleted when it is no longer required.
	 * Drawing is clipped only to the supplied rect.  If the rect is
	 * not within the region of this gadget graphical anomalies will occur.
	 * This should only be called by the Woopsi hierarchy.
	 *
	 * @param clipRect The region to clip to.
	 * @return A pointer to a new GraphicsPort object.
	 */
	virtual GraphicsPort* newGraphicsPort(Rect clipRect);

	/**
	 * Gets a pointer to the vector of all of the visible regions of this gadget.
	 * @return A pointer to a vector of all visible regions.
	 */
	DynamicArray<Rect>* getVisibleRectCache();

	/**
	 * Gets a pointer to the gadget's font.
	 * @return A pointer to the gadget's font.
	 */
	FontBase* getFont();

	/**
	 * Gets the colour used as the background fill.
	 * @return Background fill colour.
	 */
	const u16 getBackColour() const;

	/**
	 * Gets the colour used as the light edge in bevelled boxes.
	 * @return Shine colour.
	 */
	const u16 getShineColour() const;

	/**
	 * Gets the colour used as the fill in focused window borders.
	 * @return Highlight colour.
	 */
	const u16 getHighlightColour() const;

	/**
	 * Gets the colour used as the dark edge in bevelled boxes.
	 * @return Shadow colour.
	 */
	const u16 getShadowColour() const;

	/**
	 * Gets the colour used as the fill in unfocused window borders.
	 * @return Fill colour.
	 */
	const u16 getFillColour() const;

	/**
	 * Gets the colour used as the fill in scrollbar gutters.
	 * @return Dark colour.
	 */
	const u16 getDarkColour() const;

	/**
	 * Gets the type of outline used in this gadget.
	 * @return Outline type.
	 */
	const OutlineType getOutlineType() const;

	/**
	 * Sets this gadget's reference constant.  This should be unique,
	 * at least amongst this gadget's siblings.
	 * @param refcon The reference constant.
	 */
	u32 setRefcon(u32 refcon);

	/**
	 * Sets this gadget's border state.  This does not redraw the gadget.
	 * @param isBorderless The border state.
	 */
	void setBorderless(bool isBorderless);

	/**
	 * Sets whether or not this gadget can be dragged.
	 * @param isDraggable The draggable state.
	 */
	void setDraggable(bool isDraggable);

	/**
	 * Sets whether or not child gadgets can exceed this gadget's dimensions.
	 * @param isPermeable The permeable state.
	 */
	void setPermeable(bool isPermeable);

	/**
	 * Sets the outline type to use when drawing the gadget's border.
	 * @param outline The outline type.
	 */
	void setOutlineType(OutlineType outline);

	/**
	 * Sets the gadget's event handler.  The event handler will receive
	 * all events raised by this gadget.
	 * @param eventHandler A pointer to the event handler.
	 */
	void setEventHandler(EventHandler* eventHandler);

	/**
	 * Enables or disables event firing for this gadget.
	 * @param firesEvents True to enable events, false to disable.
	 */
	void setFiresEvents(const bool firesEvents);

	/**
	 * Sets the gadget visible or invisible. This does not redraw or
	 * erase the gadget.  Gadgets hidden using this method will still
	 * be processed.
	 *
	 * @param visible The visibility state.
	 */
	void setVisible(bool visible);

	/**
	 * Sets the background colour.
	 * @param colour The new background colour.
	 */
	void setBackColour(u16 colour);

	/**
	 * Sets the shine colour.
	 * @param colour The new shine colour.
	 */
	void setShineColour(u16 colour);

	/**
	 * Sets the highlight colour.
	 * @param colour The new highlight colour.
	 */
	void setHighlightColour(u16 colour);

	/**
	 * Sets the shadow colour.
	 * @param colour The new shadow colour.
	 */
	void setShadowColour(u16 colour);

	/**
	 * Sets the fill colour.
	 * @param colour The new fill colour.
	 */
	void setFillColour(u16 colour);

	/**
	 * Sets the dark colour.
	 * @param colour The new dark colour.
	 */
	void setDarkColour(u16 colour);

	/**
	 * Sets the close type other gadgets should use when closing this gadget.
	 * @param closeType The close type to use.
	 */
	void setCloseType(CloseType closeType);

	/**
	 * Sets the font.
	 * @param font A pointer to the font to use.
	 */
	virtual void setFont(FontBase* font);

	/**
	 * Draws the visible regions of the gadget and the gadget's child gadgets.
	 */
	virtual void draw();

	/**
	 * Erases the visible regions of the gadget by redrawing the gadgets
	 * behind it.
	 */
	virtual void erase();

	/**
	 * Enables the gadget.
	 * @return True if the gadget was enabled.
	 */
	virtual bool enable();

	/**
	 * Disabled the gadget.
	 * @return True if the gadget was disabled.
	 */
	virtual bool disable();

	/**
	 * Erases the gadget, marks it as deleted, and moves it to Woopsi's
	 * deletion queue.  Gadgets are automatically deleted by the framework and
	 * should not be deleted externally.
	 */
	virtual void close();

	/**
	 * Erases the gadget, and sets it to invisible.  Gadgets hidden in
	 * this way will be partioned off from other gadgets and will no
	 * longer be processed.
	 *
	 * @return True if the gadget was hidden.
	 * @see show()
	 */
	virtual bool hide();

	/**
	 * Sets the gadget to visible and redraws it.  Gadgets shown in this way
	 * will be unpartioned and will be processed again.
	 *
	 * @return True if the gadget was shown.
	 * @see hide()
	 */
	virtual bool show();

	/**
	 * Click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y);

	/**
	 * Release this gadget at the supplied co-ordinates
	 * @param x X co-ordinate of the release.
	 * @param y Y co-ordinate of the release.
	 * @return True if the release was successful.
	 */
	virtual bool release(s16 x, s16 y);

	/**
	 * Drag the gadget to the supplied co-ordinates.
	 * @param x The x co-ordinate of the stylus.
	 * @param y The y co-ordinate of the stylus.
	 * @param vX The horizontal distance that the stylus was dragged.
	 * @param vY The vertical distance that the stylus was dragged.
	 * @return True if the drag was successful.
	 */
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	/**
	 * Run any code that should execute every VBL.
	 * @return True if the function ran succesfully.
	 */
	virtual bool vbl();

	/**
	 * Send a keypress to the gadget.
	 * @param keyCode The keycode to send to the gadget.
	 * @return True if the keypress was processed.
	 */
	virtual bool keyPress(KeyCode keyCode);

	/**
	 * Send a key release to the gadget.
	 * @param keyCode The keycode to send to the gadget.
	 * @return True if the key release was processed.
	 */
	virtual bool keyRelease(KeyCode keyCode);

	/**
	 * Inform the gadget that the lid has closed.
	 * @see lidOpened()
	 */
	virtual void lidClosed();

	/**
	 * Inform the gadget that the lid has opened.
	 * @see lidClosed()
	 */
	virtual void lidOpened();

	/**
	 * Give the gadget focus.
	 * @return True if the gadget received focus correctly.
	 */
	virtual bool focus();

	/**
	 * Remove focus from the gadget.
	 * @return True if the gadget lost focus correctly.
	 */
	virtual bool blur();

	/**
	 * Move the gadget to the new co-ordinates.
	 * Co-ordinates are relative to the parent gadget.
	 * @param x The new x co-ordinate.
	 * @param y The new y co-ordinate.
	 * @return True if the move was successful.
	 */
	virtual bool moveTo(s16 x, s16 y);

	/**
	 * Resize the gadget to the new dimensions.
	 * @param width The new width.
	 * @param height The new height.
	 * @return True if the resize was successful.
	 */
	virtual bool resize(u16 width, u16 height);

	/**
	 * Raises the gadget to the top of its parent's gadget stack.
	 * @return True if the raise was successful.
	 */
	virtual bool raiseToTop();

	/**
	 * Lowers the gadget to the bottom of its parent's gadget stack.
	 * @return True if the lower was successful.
	 */
	virtual bool lowerToBottom();

	/**
	 * Raises the supplied gadget to the top of this gadget's child stack.
	 * The supplied gadget pointer must be a child of this gadget.
	 * @param gadget A pointer to the child gadget to raise.
	 * @return True if the raise was successful.
	 */
	virtual bool raiseGadgetToTop(Gadget* gadget);

	/**
	 * Lowers the supplied gadget to the bottom of this gadget's child stack.
	 * The supplied gadget pointer must be a child of this gadget.
	 * @param gadget A pointer to the child gadget to lower.
	 * @return True if the lower was successful.
	 */
	virtual bool lowerGadgetToBottom(Gadget* gadget);

	/**
	 * Moves the supplied child gadget to the deletion queue.
	 * For framework use only.
	 * @param gadget A pointer to the child gadget.
	 */
	void moveChildToDeleteQueue(Gadget* gadget);

	/**
	 * Moves the supplied child gadget to the hidden gadget list.
	 * For framework use only.
	 * @param gadget A pointer to the child gadget.
	 * @return True if the gadget was moved successfully.
	 * @see moveHiddenToChildList()
	 * @see hide()
	 */
	bool moveChildToHiddenList(Gadget* gadget);

	/**
	 * Moves the supplied child gadget from the hidden list back
	 * to the child gadget list.
	 * For framework use only.
	 * @param gadget A pointer to the hidden gadget.
	 * @return True if the gadget was moved successfully.
	 * @see moveChildtoHiddenList()
	 * @see show()
	 */
	bool moveHiddenToChildList(Gadget* gadget);

	/**
	 * Sets the supplied gadget as the active child.  The gadget must
	 * be a child of this gadget.
	 * @param gadget A pointer to the child gadget.
	 * @see getActiveGadget()
	 */
	virtual void setActiveGadget(Gadget* gadget);

	/**
	 * Sets the supplied gadget as the clicked child.  The gadget must
	 * be a child of this gadget.
	 * @param gadget A pointer to the child gadget.
	 * @see getClickedGadget()
	 */
	virtual void setClickedGadget(Gadget* gadget);

	/**
	 * Checks if the supplied co-ordinates collide with this gadget.
	 * @param x The x co-ordinate to check.
	 * @param y The y co-ordinate to check.
	 * @return True if a collision occurred.
	 */
	bool checkCollision(s16 x, s16 y);

	/**
	 * Checks if the supplied rectangle definition collides with this gadget.
	 * @param x The x co-ordinate of the rectangle to check.
	 * @param y The y co-ordinate of the rectangle to check.
	 * @param width The width of the rectangle to check.
	 * @param height The height of the rectangle to check.
	 * @return True if a collision occurred.
	 */
	bool checkCollision(s16 x, s16 y, u16 width, u16 height);

	/**
	 * Checks if the supplied gadget collides with this gadget.
	 * @param gadget A pointer to another gadget to check for collisions with.
	 * @return True if a collision occurred.
	 */
	bool checkCollision(Gadget* gadget);

	/**
	 * Invalidate the visible region cache for all gadgets below the supplied
	 * gadget in this gadget's child stack.  This will cause those gadgets to
	 * recalculate their visible regions next time they try to draw themselves.
	 * @param A pointer to a child gadget.
	 */
	void invalidateLowerGadgetsVisibleRectCache(Gadget* gadget);

	/**
	 * Removes all of this gadget's children from the list of gadgets that
	 * receive VBL notifications.
	 */
	void unregisterChildrenFromVBL();

	/**
	 * Adds a gadget to this gadget's child stack.  The gadget is added to the
	 * top of the stack.  Note that the gadget can only be added if it is not
	 * already a child of another gadget.
	 * @param gadget A pointer to the gadget to add to the child list.
	 * @see insertGadget()
	 */
	void addGadget(Gadget* gadget);

	/**
	 * Inserts a gadget into this gadget's child stack at the bottom of the
	 * stack.  Note that the gadget can only be added if it is not already
	 * a child of another gadget.
	 * @param gadget A pointer to the gadget to add to the child list.
	 * @see addGadget()
	 */
	void insertGadget(Gadget* gadget);

	/**
	 * Set the gadget's parent to the gadget passed in as a parameter.
	 * Called automatically when a gadget is added as a child.
	 * @param parent A pointer to the parent gadget.
	 */
	void setParent(Gadget* parent);

	/**
	 * Notify this gadget that it is being dragged, and set its drag point.
	 * @param x The x co-ordinate of the drag position relative to this gadget.
	 * @param y The y co-ordinate of the drag position relative to this gadget.
	 */
	void setDragging(u16 x, u16 y);

	/**
	 * Rebuild the list of this gadget's visible regions
	 */
	void cacheVisibleRects();

	/**
	 * Mark this gadget's visible region cache as invalid, and do the same
	 * to its child gadgets.
	 */
	void invalidateVisibleRectCache();

	/**
	 * Draw the area of this gadget that falls within the clipping region.
	 * Called by the draw() function to draw all visible regions.
	 * @param clipRect The clipping region to draw.
	 * @see draw()
	 */
	virtual void draw(Rect clipRect);

	/**
	 * Erase a child gadget by drawing the gadgets behind it.
	 * @param gadget The child gadget to erase.
	 */
	virtual void eraseGadget(Gadget* gadget);

	/**
	 * Redraw any visible regions of this gadget that have become corrupted.
	 * @param invalidRects A list of corrupt regions.
	 * @param sender A pointer to the gadget that corrupted the regions.
	 */
	virtual void redrawDirty(DynamicArray<Rect>* invalidRects, Gadget* sender);

	/**
	 * Move any rectangles from the visibleRects list that overlap this gadget
	 * into the invisibleRects list.  Used during visible region calculations.
	 * @param visibleRects A vector of regions that are not overlapped.
	 * @param invisibleRects A vector of regions that are overlapped.
	 * @param gadget The gadget that requested the lists.
	 * @see splitRectangles()
	 */
	virtual void removeOverlappedRects(DynamicArray<Rect>* visibleRects, DynamicArray<Rect>* invisibleRects, Gadget* gadget);

	/**
	 * Works out which rectangles in the invalidRectangles list overlap this
	 * gadget, then cuts the rectangles into smaller pieces.  The overlapping
	 * pieces are pushed into validRects, and the non-overlapping pieces are
	 * pushed back into the invalidRects vector.
	 * @param invalidRectangles A vector of regions that need to be tested
	 * for collisions against this gadget; they represent regions that need
	 * to be redrawn.
	 * @param validRects A vector of regions that represents areas of the
	 * display that do not need to be redrawn.
	 */
	virtual void splitRectangles(DynamicArray<Rect>* invalidRectangles, DynamicArray<Rect>* validRects, Gadget* sender);

	/**
	 * Clips a rectangular region to the dimensions of this gadget and its ancestors.
	 * @param rect The region that needs to be clipped.
	 * @param sender A pointer to the gadget that called this function.
	 */
	void clipRectToHierarchy(Rect& rect, Gadget* sender);

	/**
	 * Swaps the depth of the supplied child gadget.
	 * @param gadget A pointer to the child gadget that needs to swap depths.
	 * @retun True if the swap was successful.
	 */
	virtual bool swapGadgetDepth(Gadget* gadget);

	/**
	 * Swap the depth of this gadget.
	 * @return True if the swap was successful.
	 */
	bool swapDepth();

protected:
	s16 _x;
	s16 _y;
	u16 _width;
	u16 _height;
	u32 _refcon;

	// Dragging variables
	s16 _grabPointX;
	s16 _grabPointY;
	s16 _newX;
	s16 _newY;

	// Colour definitions
	u16 _backColour;
	u16 _shineColour;
	u16 _highlightColour;
	u16 _shadowColour;
	u16 _fillColour;
	u16 _darkColour;

	// Status
	Flags _flags;

	// Event handling
	EventHandler* _eventHandler;

	// Hierarchy control
	Gadget* _parent;
	Gadget* _activeGadget;
	Gadget* _clickedGadget;
	DynamicArray<Gadget*> _gadgets;
	DynamicArray<Gadget*> _hiddenGadgets;

	// Decorations
	u8 _decorationCount;

	// Visible regions
	DynamicArray<Rect> _visibleRegionCache;
	bool _visibleRegionCacheInvalid;

	OutlineType _outline;
	CloseType _closeType;

	FontBase* _font;

	// Initialisation
	void init();

	const s16 calculatePhysicalScreenY(s16 y) const;
	const u8 calculatePhysicalScreenNumber(s16 y) const;

	// Drawing tools
	void clear(Rect clipRect);
	void clear();
	void drawChildren();

	s16 getGadgetIndex(Gadget* gadget);

	virtual void setActive(bool active);

	virtual void closeChild(Gadget* gadget);
	virtual void hideChild(Gadget* gadget);

	virtual void redrawDirtyChildren(DynamicArray<Rect>* invalidRects, Gadget* sender);

	virtual GraphicsPort* newInternalGraphicsPort();
	virtual GraphicsPort* newInternalGraphicsPort(Rect clipRect);

	// Events
	void raiseClickEvent(s16 x, s16 y);
	void raiseReleaseEvent(s16 x, s16 y);
	void raiseDragEvent(s16 x, s16 y, s16 vX, s16 vY);
	void raiseVBLEvent();
	void raiseKeyPressEvent(KeyCode keyCode);
	void raiseKeyReleaseEvent(KeyCode keyCode);
	void raiseLidClosedEvent();
	void raiseLidOpenedEvent();
	void raiseFocusEvent();
	void raiseBlurEvent();
	void raiseCloseEvent();
	void raiseHideEvent();
	void raiseShowEvent();
	void raiseEnableEvent();
	void raiseDisableEvent();
	void raiseValueChangeEvent();
	void raiseResizeEvent(u16 width, u16 height);
	void raiseMoveEvent(s16 x, s16 y);
};

#endif
