#ifndef _GADGET_H_
#define _GADGET_H_

#include <nds.h>
#include "defines.h"
#include "gadgeteventhandler.h"
#include "woopsiarray.h"
#include "glyphs.h"
#include "rect.h"
#include "fontbase.h"
#include "gadgetstyle.h"
#include "gadgeteventhandlerlist.h"

namespace WoopsiUI {

	class GraphicsPort;
	class FontBase;
	class RectCache;

	/**
	 * Class providing all the basic functionality of a Woopsi gadget.
	 * All Woopsi gadgets must inherit from this class.
	 */
	class Gadget {
	public:

		/**
		 * Enum describing the types of border around a gadget.
		 */
		enum OutlineType {
			OUTLINE_CLICK_DEPENDENT = 0,		/**< Border depends on current click status */
			OUTLINE_OUT = 1,					/**< Border is bevelled "out" */
			OUTLINE_IN = 2,						/**< Border is bevelled *in* */
			OUTLINE_OUT_IN = 3					/**< Border is bevelled out and then in */
		};

		/**
		 * Enum describing the way other gadgets should behave when they try to close this gadget.
		 */
		enum CloseType {
			CLOSE_TYPE_CLOSE = 0,				/**< Gadgets should call the close() method */
			CLOSE_TYPE_HIDE = 1,				/**< Gadgets should call the hide() method */
			CLOSE_TYPE_SHELVE = 2				/**< Gadgets should call the shelve() method */
		};

		/**
		 * Enum listing flags that can be set in the constructor's "flags" parameter.
		 */
		enum GadgetFlagType {
			GADGET_BORDERLESS = 0x0001,					/**< Gadget has no border */
			GADGET_DRAGGABLE = 0x0002,					/**< Gadget can be dragged by the user */
			GADGET_PERMEABLE = 0x0004,					/**< Gadget's children can exceed this gadget's edges */
			GADGET_DOUBLE_CLICKABLE = 0x0008,			/**< Gadget can be double-clicked */
			GADGET_NO_SHIFT_CLICK_CHILDREN = 0x0010,	/**< Gadget does not send shift clicks to children */
			GADGET_NO_RAISE_EVENTS = 0x0020,			/**< Gadget does not raise events */
			GADGET_DECORATION = 0x0040					/**< Gadget is a decoration */
		};

		/**
		 * Struct describing some basic properties of a gadget.
		 */
		typedef struct {
			u8 clicked : 1;						/**< True if the gadget is currently clicked. */
			u8 hasFocus : 1;					/**< True if the gadget has focus. */
			u8 dragging : 1;					/**< True if the gadget is being dragged. */
			u8 deleted : 1;						/**< True if the gadget has been deleted. */
			u8 shelved : 1;						/**< True if the gadget has been shelved. */
			u8 borderless : 1;					/**< True if the gadget is borderless. */
			u8 draggable : 1;					/**< True if the gadget can be dragged. */
			u8 drawingEnabled : 1;				/**< True if the gadget can be drawn. */
			u8 enabled : 1;						/**< True if the gadget is enabled. */
			u8 decoration : 1;					/**< True if the gadget is a decoration. */
			u8 permeable : 1;					/**< True if the gadget's children can exceed its dimensions. */
			u8 erased : 1;						/**< True if the gadget is currently erased from the frame buffer. */
			u8 shiftClickChildren : 1;			/**< True if the gadget sends shift clicks to its children. */
			u8 visibleRegionCacheInvalid : 1;	/**< True if the region cache is invalid. */
			u8 hidden : 1;						/**< True if the gadget is hidden. */
			u8 doubleClickable : 1;				/**< True if the gadget can be double-clicked. */
			u8 modal : 1;						/**< True if the gadget is modal. */
		} Flags;

		/**
		 * Struct describing a name/value pair.
		 */
		typedef struct {
			u32 value;							/**< Value associated with the name */
			char* name;							/**< Name associated with the value */
		} NameValuePair;

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param flags Bitmask specifying some set-up values for the gadget.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 * data.  If no object is specified the gadget will use the default style.
		 * @see GadgetFlagType.
		 */
		Gadget(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style = NULL);
		
		/**
		 * Get the x co-ordinate of the gadget in "Woopsi space".
		 * @return Woopsi space x co-ordinate.
		 */
		const s16 getX() const;

		/**
		 * Get the y co-ordinate of the gadget in "Woopsi space".
		 * @return Woopsi space y co-ordinate.
		 */
		const s16 getY() const;

		/**
		 * Get the reference constant for this gadget.
		 * @return The reference constant.
		 */
		inline const u32 getRefcon() const { return _refcon; };

		/**
		 * Get the type of close routine that should be called by other gadgets
		 * interacting with this gadget.
		 * @return The close type of this gadget.
		 */
		inline const CloseType getCloseType() { return _closeType; };

		/**
		 * Is the gadget active?
		 * A value of true indicates that this gadget has focus or is an ancestor
		 * of the gadget with focus.
		 * @return True if active.
		 */
		inline const bool hasFocus() const { return _flags.hasFocus; };

		/**
		 * Has the gadget been marked for deletion?  This function recurses up the gadget
		 * hierarchy and only returns true if all of the gadgets in the ancestor
		 * chain are not deleted.  
		 * Gadgets marked for deletion are automatically deleted and should not be
		 * interacted with.
		 * @return True if marked for deletion.
		 */
		const bool isDeleted() const;

		/**
		 * Is the gadget allowed to draw?  This function recurses up the gadget
		 * hierarchy and only returns true if all of the gadgets in the ancestor
		 * chain are visible.
		 * @return True if drawing is enabled.
		 */
		const bool isDrawingEnabled() const;

		/**
		 * Is the gadget hidden?  This function recurses up the gadget
		 * hierarchy and returns true if any of the gadgets in the ancestor
		 * chain are hidden.
		 * @return True if hidden.
		 */
		const bool isHidden() const;

		/**
		 * Is the gadget enabled?  This function recurses up the gadget
		 * hierarchy and only returns true if all of the gadgets in the ancestor
		 * chain are enabled.
		 * @return True if enabled.
		 */
		const bool isEnabled() const;

		/**
		 * Is the gadget a decoration?
		 * Decoration gadgets are children of, but also an essential component of,
		 * another gadget.
		 * @return True if the gadget is a decoration.
		 */
		inline const bool isDecoration() const { return _flags.decoration; };

		/**
		 * Are the gadget's edges permeable or solid?
		 * Permeable gadgets do not enforce their dimensions on the
		 * co-ordinates and dimensions of child gadgets.
		 * @return True if permeable.
		 */
		inline const bool isPermeable() const { return _flags.permeable; };

		/**
		 * IS the gadget double-clickable?
		 * @return True if the gadget watches for double-clicks.
		 */
		inline const bool isDoubleClickable() const { return _flags.doubleClickable; };

		/**
		 * Does the gadget have a border?
		 * @return True if the gadget does not have a border.
		 */
		inline const bool isBorderless() const { return _flags.borderless; };

		/**
		 * Is the gadget clicked?
		 * @return True if the gadget is currently clicked.
		 */
		inline const bool isClicked() const { return _flags.clicked; };

		/**
		 * Is the gadget shelved?
		 * @return True if the gadget is shelved.
		 */
		inline const bool isShelved() const { return _flags.shelved; };

		/**
		 * Is the gadget modal?  Only true if the Woopsi singleton is also modal.
		 * @return True if the gadget is modal.
		 */
		const bool isModal() const;

		/**
		 * Does the gadget shift-click its children?
		 * @return True if the gadget shift-clicks its children.
		 */
		inline const bool getShiftClickChildren() const { return _flags.shiftClickChildren; };

		/**
		 * Does the gadget shift-click its children?
		 * @return True if the gadget shift-clicks its children.
		 */
		inline void setShiftClickChildren(const u32 shiftClickChildren) { _flags.shiftClickChildren = shiftClickChildren; };

		/**
		 * Get the width of the gadget.
		 * @return The gadget width.
		 */
		inline u16 getWidth() const { return _width; };

		/**
		 * Get the height of the gadget.
		 * @return The gadget height.
		 */
		inline u16 getHeight() const { return _height; };

		/**
		 * Get a pointer to this gadget's parent.
		 * @return This gadget's parent.
		 */
		inline Gadget* getParent() const { return _parent; };

		/**
		 * Get a pointer to this gadget's focused child.
		 * @return This gadget's focused child.
		 */
		inline Gadget* getFocusedGadget() { return _focusedGadget; };

		/**
		 * Get the number of the screen that this gadget is currently displayed on.
		 * @return 0 for the touch screen, 1 for the top screen.
		 */
		inline const u8 getPhysicalScreenNumber() const { return calculatePhysicalScreenNumber(getY()); };

		/**
		 * Check if this gadget raises events or not.
		 * @return True if events are enabled.
		 */
		inline const bool raisesEvents() const { return _gadgetEventHandlers->isEnabled(); };

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the gadget's
		 * parent.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

		/**
		 * Insert the properties of the space within this gadget that is available
		 * for children into the rect passed in as a parameter.
		 * All co-ordinates are relative to this gadget.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getClientRect(Rect& rect) const;

		/**
		 * Clips the supplied rect to the boundaries defined by this gadget and
		 * this gadget's parents.
		 * @param rect Reference to a rect to populate with data.
		 */
		void getRectClippedToHierarchy(Rect& rect) const;

		/**
		 * Gets a pointer to a new instance of the GraphicsPort class to allow
		 * drawing within this gadget's client space.
		 * The GraphicsPort object must be deleted when it is no longer required.
 		 * @param isForeground True to use the foreground clipping list (draw
		 * over all space, including that overlapped by children) or background
		 * clipping list (not including that overlapped by children).
		 * @return A pointer to a new GraphicsPort object.
		 */
		virtual GraphicsPort* newGraphicsPort(bool isForeground);

		/**
		 * Gets a pointer to a new instance of the GraphicsPort class to allow
		 * drawing within this gadget's client space.  The GraphicsPort can
		 * only draw within the supplied region.
		 *
		 * The GraphicsPort object must be deleted when it is no longer required.
		 * Drawing is clipped only to the supplied rect.  If the rect is
		 * not within the region of this gadget graphical anomalies will occur.
		 * This should only be called by the Woopsi hierarchy.
		 * @param clipRect The region to clip to.
		 * @return A pointer to a new GraphicsPort object.
		 */
		virtual GraphicsPort* newGraphicsPort(Rect clipRect);

		/**
		 * Gets a pointer to the vector of all of the visible regions of this gadget,
		 * including any covered by children.
		 * @return A pointer to a vector of all visible regions.
		 */
		WoopsiArray<Rect>* getForegroundRegions();

		/**
		 * Gets a pointer to the vector of all of the visible regions of this gadget,
		 * not including any covered by children.
		 * @return A pointer to a vector of all visible regions not covered by children.
		 */
		WoopsiArray<Rect>* getBackgroundRegions();

		/**
		 * Gets a pointer to the gadget's font.
		 * @return A pointer to the gadget's font.
		 */
		FontBase* getFont() const;

		/**
		 * Gets the colour used as the background fill.
		 * @return Background fill colour.
		 */
		inline const u16 getBackColour() const { return _style->colours.back; };

		/**
		 * Gets the colour used as the light edge in bevelled boxes.
		 * @return Shine colour.
		 */
		inline const u16 getShineColour() const { return _style->colours.shine; };

		/**
		 * Gets the colour used as the fill in focused window borders.
		 * @return Highlight colour.
		 */
		inline const u16 getHighlightColour() const { return _style->colours.highlight; };

		/**
		 * Gets the colour used as the dark edge in bevelled boxes.
		 * @return Shadow colour.
		 */
		inline const u16 getShadowColour() const { return _style->colours.shadow; };

		/**
		 * Gets the colour used as the fill in unfocused window borders.
		 * @return Fill colour.
		 */
		inline const u16 getFillColour() const { return _style->colours.fill; };

		/**
		 * Gets the colour used as the fill in scrollbar gutters.
		 * @return Dark colour.
		 */
		inline const u16 getDarkColour() const { return _style->colours.dark; };

		/**
		 * Gets the type of outline used in this gadget.
		 * @return Outline type.
		 */
		inline const OutlineType getOutlineType() const { return _outline; };

		/**
		 * Sets this gadget's reference constant.  This should be unique,
		 * at least amongst this gadget's siblings.
		 * @param refcon The reference constant.
		 */
		u32 setRefcon(u32 refcon);

		/**
		 * Sets this gadget's border state.
		 * @param isBorderless The border state.
		 */
		void setBorderless(bool isBorderless);

		/**
		 * Sets whether or not this gadget can be dragged.
		 * @param isDraggable The draggable state.
		 */
		inline void setDraggable(const bool isDraggable) { _flags.draggable = isDraggable; };

		/**
		 * Sets whether or not child gadgets can exceed this gadget's dimensions.
		 * @param isPermeable The permeable state.
		 */
		inline void setPermeable(const bool isPermeable) { _flags.permeable = isPermeable; };

		/**
		 * Sets whether or not the gadgets processes double-clicks.
		 * @param isDoubleClickable The double-clickable state.
		 */
		inline void setDoubleClickable(const bool isDoubleClickable) { _flags.doubleClickable = isDoubleClickable; };

		/**
		 * Sets the outline type to use when drawing the gadget's border.
		 * @param outline The outline type.
		 */
		inline void setOutlineType(const OutlineType outline) { _outline = outline; };

		/**
		 * Adds a gadget event handler.  The event handler will receive
		 * all events raised by this gadget.
		 * @param eventHandler A pointer to the event handler.
		 */
		inline void addGadgetEventHandler(GadgetEventHandler* eventHandler) { _gadgetEventHandlers->addGadgetEventHandler(eventHandler); };

		/**
		 * Remove a gadget event handler.
		 * @param eventHandler A pointer to the event handler to remove.
		 */
		inline void removeGadgetEventHandler(GadgetEventHandler* eventHandler) { _gadgetEventHandlers->removeGadgetEventHandler(eventHandler); };

		/**
		 * Enables or disables event firing for this gadget.
		 * @param raisesEvents True to enable events, false to disable.
		 */
		inline void setRaisesEvents(const bool raisesEvents) { 
			raisesEvents ? _gadgetEventHandlers->enable() : _gadgetEventHandlers->disable();
		};

		/**
		 * Disabled drawing of this gadget. Gadgets hidden using this method will still
		 * be processed.
		 */
		inline void disableDrawing() { _flags.drawingEnabled = false; };

		/**
		 * Enables drawing of this gadget.
		 */
		inline void enableDrawing() { _flags.drawingEnabled = true; };

		/**
		 * Sets the background colour.
		 * @param colour The new background colour.
		 */
		inline void setBackColour(const u16 colour) { _style->colours.back = colour; };

		/**
		 * Sets the shine colour.
		 * @param colour The new shine colour.
		 */
		inline void setShineColour(const u16 colour) { _style->colours.shine = colour; };

		/**
		 * Sets the highlight colour.
		 * @param colour The new highlight colour.
		 */
		inline void setHighlightColour(const u16 colour) { _style->colours.highlight = colour; };

		/**
		 * Sets the shadow colour.
		 * @param colour The new shadow colour.
		 */
		inline void setShadowColour(const u16 colour) { _style->colours.shadow = colour; };

		/**
		 * Sets the fill colour.
		 * @param colour The new fill colour.
		 */
		inline void setFillColour(const u16 colour) { _style->colours.fill = colour; };

		/**
		 * Sets the dark colour.
		 * @param colour The new dark colour.
		 */
		inline void setDarkColour(const u16 colour) { _style->colours.dark = colour; };

		/**
		 * Sets the close type other gadgets should use when closing this gadget.
		 * @param closeType The close type to use.
		 */
		inline void setCloseType(const CloseType closeType) { _closeType = closeType; };

		/**
		 * Sets the font.
		 * @param font A pointer to the font to use.
		 */
		virtual void setFont(FontBase* font);

		/**
		 * Draws the visible regions of the gadget and the gadget's child gadgets.
		 */
		virtual void redraw();

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
		 * Erases the gadget, removes it from the main hierarchy and sets it to
		 * invisible.  Gadgets hidden in this way will be partioned off from
		 * other gadgets and will no longer be processed.
		 * @return True if the gadget was shelved.
		 * @see unshelve()
		 */
		virtual bool shelve();

		/**
		 * Moves the gadget back into the hierarchy and redraws it.  Gadgets shown
		 * in this way will be unpartioned and will be processed again.
		 * @return True if the gadget was unshelved.
		 * @see shelve()
		 */
		virtual bool unshelve();

		/**
		 * Draws the gadget and makes it visible.
		 * Does not steal focus from other gadgets.
		 * @return True if the gadget was shown.
		 * @see hide()
		 */
		virtual bool show();

		/**
		 * Erases the gadget and makes it invisible.
		 * Does not re-assign focus to another gadget.
		 * @return True if the gadget was hidden.
		 * @see show()
		 */
		virtual bool hide();

		/**
		 * Click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		virtual bool click(s16 x, s16 y);

		/**
		 * Check if the click is a double-click
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click is a double-click.
		 */
		virtual bool isDoubleClick(s16 x, s16 y);

		/**
		 * Double-click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		virtual bool doubleClick(s16 x, s16 y);

		/**
		 * Shift-click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		virtual bool shiftClick(s16 x, s16 y);

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
		virtual void lidClose();

		/**
		 * Inform the gadget that the lid has opened.
		 * @see lidClosed()
		 */
		virtual void lidOpen();

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
		 * Resize and move the gadget in one operation.
		 * Only performs one redraw so it is faster than calling the
		 * two separate functions.
		 * @param x The new x co-ordinate.
		 * @param y The new y co-ordinate.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the gadget was adjusted successfully.
		 */
		virtual bool changeDimensions(s16 x, s16 y, u16 width, u16 height);

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
		 * Moves the supplied child gadget to the shelved gadget list.
		 * For framework use only.
		 * @param gadget A pointer to the child gadget.
		 * @return True if the gadget was moved successfully.
		 * @see moveShelvedToChildList()
		 * @see hide()
		 */
		bool moveChildToShelvedList(Gadget* gadget);

		/**
		 * Moves the supplied child gadget from the shelved list back
		 * to the child gadget list.
		 * For framework use only.
		 * @param gadget A pointer to the shelved gadget.
		 * @return True if the gadget was moved successfully.
		 * @see moveChildtoShelvedList()
		 * @see show()
		 */
		bool moveShelvedToChildList(Gadget* gadget);

		/**
		 * Sets the supplied gadget as the focused child.  The gadget must
		 * be a child of this gadget.
		 * @param gadget A pointer to the child gadget.
		 * @see getFocusedGadget()
		 */
		virtual void setFocusedGadget(Gadget* gadget);

		/**
		 * Checks if the supplied co-ordinates collide with this gadget.
		 * @param x The x co-ordinate to check.
		 * @param y The y co-ordinate to check.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(s16 x, s16 y) const;

		/**
		 * Checks if the supplied rectangle definition collides with this gadget.
		 * @param x The x co-ordinate of the rectangle to check.
		 * @param y The y co-ordinate of the rectangle to check.
		 * @param width The width of the rectangle to check.
		 * @param height The height of the rectangle to check.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(s16 x, s16 y, u16 width, u16 height) const;

		/**
		 * Checks if the supplied gadget collides with this gadget.
		 * @param gadget A pointer to another gadget to check for collisions with.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(Gadget* gadget) const;

		/**
		 * Invalidate the visible region cache for all gadgets below the supplied
		 * gadget in this gadget's child stack.  This will cause those gadgets to
		 * recalculate their visible regions next time they try to draw themselves.
		 * @param gadget A pointer to a child gadget.
		 */
		void invalidateLowerGadgetsVisibleRectCache(Gadget* gadget);

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
		inline void setParent(Gadget* parent) { _parent = parent; };

		/**
		 * Notify this gadget that it is being dragged, and set its drag point.
		 * @param x The x co-ordinate of the drag position relative to this gadget.
		 * @param y The y co-ordinate of the drag position relative to this gadget.
		 */
		virtual void setDragging(u16 x, u16 y);

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
		virtual inline void draw(Rect clipRect) { };

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
		virtual void redrawDirty(WoopsiArray<Rect>* invalidRects, Gadget* sender);

		/**
		 * Clips a rectangular region to the dimensions of this gadget and its ancestors.
		 * @param rect The region that needs to be clipped.
		 */
		void clipRectToHierarchy(Rect& rect) const;

		/**
		 * Swaps the depth of the supplied child gadget.
		 * @param gadget A pointer to the child gadget that needs to swap depths.
		 * @return True if the swap was successful.
		 */
		virtual bool swapGadgetDepth(Gadget* gadget);

		/**
		 * Swap the depth of this gadget.
		 * @return True if the swap was successful.
		 */
		bool swapDepth();

		/**
		 * Delete this gadget.  This should never be called in user code; gadget
		 * deletion is handled by Woopsi.
		 */
		inline void destroy() { delete this; };

		/**
		 * Remove this gadget from Woopsi's gadget hierarchy.  Returns
		 * responsibility for deleting the gadget back to the developer.
		 * Does not unregister the gadget from the VBL system.
		 * Does not erase the gadget from the display.
		 * @return True if the gadget was successfully removed.
		 */
		bool remove();

		/**
		 * Remove a child gadget from Woopsi's gadget hierarchy.  Returns
		 * responsibility for deleting the gadget back to the developer.
		 * Does not unregister the gadget from the VBL system.
		 * Does not erase the gadget from the display.
		 * @param gadget Pointer to the gadget to remove from the hierarchy.
		 * @return True if the gadget was succesfully removed.
		 */
		bool removeChild(Gadget* gadget);

		/**
		 * Add a context menu item definition to the gadget.
		 * @param name The name of the menu item.
		 * @param value The value of the menu item.
		 */
		void addContextMenuItem(const char* name, u32 value);

		/**
		 * Show the context menu for this gadget at the specified co-ordinates.
		 * @param x The x co-ordinate of the context menu, relative to the screen.
		 * @param y The y co-ordinate of the context menu, relative to the screen.
		 */
		virtual void showContextMenu(s16 x, s16 y);

		/**
		 * Handle a context menu selection.  Just raises the event to its own handlers.
		 * @param item The selected menu item.
		 */
		virtual bool handleContextMenuSelection(const ListDataItem* item);

		/**
		 * Run the gadget modally.
		 */
		void goModal();

		/**
		 * Stop the gadget running modally.
		 */
		inline void stopModal() { _flags.modal = false; };

		/**
		 * Get the index of the specified child gadget.
		 * @param gadget The gadget to get the index of.
		 * @return The index of the gadget.  -1 if the gadget is not found.
		 */
		const s32 getGadgetIndex(const Gadget* gadget) const;

		/**
		 * Get the child gadget at the specified index.
		 * @param index Index of the child to retrieve.
		 * @return Pointer to the child at the specified index.
		 */
		const Gadget* getChild(u32 index) const;

		/**
		 * Get the quantity of child gadgets.
		 * @return The number of child gadgets belonging to this gadget.
		 */
		const s32 getChildCount() const { return _gadgets.size(); };

		/**
		 * Get the quantity of decoration gadgets.
		 * @return The number of decoration gadgets belonging to this gadget.
		 */
		const inline s32 getDecorationCount() const { return _decorationCount; };

		/**
		 * Get a pointer to the cache of visible rects.
		 * @return A pointer to the cache of visible rects.
		 */
		inline RectCache* getRectCache() const { return _rectCache; };

	protected:
		s16 _x;									/**< X co-ordinate of the gadget, relative to parent */
		s16 _y;									/**< Y co-ordinate of the gadget, relative to parent */
		u16 _width;								/**< Width of the gadget */
		u16 _height;							/**< Height of the gadget */
		u32 _refcon;							/**< Identifying number of the gadget */

		// Dragging variables
		s16 _grabPointX;						/**< Physical space x co-ordinate where dragging began */
		s16 _grabPointY;						/**< Physical space y co-ordinate where dragging began */
		s16 _newX;								/**< Physical x co-ordinate where gadget is being dragged to */
		s16 _newY;								/**< Physical y co-ordinate where gadget is being dragged to */

		// Style
		GadgetStyle* _style;					/**< All style information used by a gadget */

		// Status
		Flags _flags;							/**< Flags struct */

		// Event handling
		GadgetEventHandlerList* _gadgetEventHandlers;		/**< List of event handlers */

		// Double-clicking
		u32 _lastClickTime;						/**< VBL count when last clicked */
		s16 _lastClickX;						/**< X co-ordinate of last click */
		s16 _lastClickY;						/**< Y co-ordinate of last click */
		s16 _doubleClickBounds;					/**< Area in which a click is assumed to be a double-click */

		// Hierarchy control
		Gadget* _parent;						/**< Pointer to the gadget's parent */
		Gadget* _focusedGadget;					/**< Pointer to the child gadget that has focus */
		WoopsiArray<Gadget*> _gadgets;			/**< List of child gadgets */
		WoopsiArray<Gadget*> _shelvedGadgets;	/**< List of shelved child gadgets */

		// Decorations
		u8 _decorationCount;					/**< Total number of decoration child gadgets */

		// Visible regions
		RectCache* _rectCache;					/**< List of the gadget's visible regions */

		OutlineType _outline;					/**< Type of outline the gadget uses */
		CloseType _closeType;					/**< Type of close method that should be called for the gadget */

		// Context menu item definitions
		WoopsiArray<NameValuePair> _contextMenuItems;	/**< List of all context menu name/value pairs */

		/**
		 * Destructor.
		 */
		virtual ~Gadget();

		/**
		 * Get the current physical display co-ordinate for the supplied y co-ordinate.
		 * Woopsi treats the two displays as two viewports on the same logical space.
		 * The lower half of the range of y co-ordinates is displayed on the bottom
		 * screen, whilst the upper half of the range of y co-ordinates is displayed
		 * on the top screen.  This function converts back into a value that can be
		 * displayed on one of the screens.
		 * Should be used in conjunction with calculatePhysicalScreenNumber to 
		 * work out which screen to draw on.
		 * @param y The y co-ordinate to check.
		 * @return 
		 * falls within the top screen.
		 * @see calculatePhysicalScreenNumber()
		 */
		const s16 calculatePhysicalScreenY(s16 y) const;

		/**
		 * Get the current physical display number for the supplied y co-ordinate.
		 * Should be used in conjunction with calculatePhysicalScreenY to convert
		 * to a y co-ordinate that can be displayed.
		 * @param y The y co-ordinate to check.
		 * @return 0 if the co-ordinate falls within the bottom screen; 1 if it
		 * falls within the top screen.
		 * @see calculatePhysicalScreenY
		 */
		const u8 calculatePhysicalScreenNumber(s16 y) const;

		/**
		 * Clear a region by drawing a filled rect in the background colour.
		 * @param clipRect The region to clear.
		 */
		void clear(Rect clipRect);

		/**
		 * Clear all visible regions of the gadget by drawing a filled rect
		 * in the background colour.
		 */
		void clear();

		/**
		 * Draw all visible regions of this gadget's children.
		 */
		void drawChildren();

		/**
		 * Erase and remove the supplied child gadget from this gadget and
		 * send it to the deletion queue.
		 * @param gadget The gadget to close.
		 * @see close().
		 */
		virtual void closeChild(Gadget* gadget);

		/**
		 * Erase the supplied child gadget and move it out of the main child
		 * list into the shelved list.  The gadget remains in memory and can
		 * be restored by calling "unshelve()" on the gadget.
		 * @param gadget The gadget to hide.
		 */
		virtual void shelveChild(Gadget* gadget);

		/**
		 * Redraws all regions of child gadgets that fall within the invalidRects
		 * regions.
		 * @param invalidRects List of invalid regions that need to be redrawn.
		 * @param sender Pointer to the gadget that initiated the redraw.
		 */
		virtual void redrawDirtyChildren(WoopsiArray<Rect>* invalidRects, Gadget* sender);

		/**
		 * Get a graphics port that can draw within this gadget's region.
		 * The graphics port's drawing routines are automatically clipped to the
		 * visible areas of this gadget.  Allows drawing over all regions of
		 * the gadget, including the border.  The port must be deleted when
		 * it is no longer required.
		 * @param isForeground True to use the foreground clipping list (draw
		 * over all space, including that overlapped by children) or background
		 * clipping list (not including that overlapped by children).
		 * @return A new graphics port object.
		 */
		virtual GraphicsPort* newInternalGraphicsPort(bool isForeground);

		/**
		 * Get a graphics port that can draw within the region of the supplied
		 * clipping rect.  The port must be deleted when it is no longer required.
		 * Note that the clipping rect should be clipped to the gadget's visible
		 * region before creating the graphics port.
		 * @return A new graphics port object.
		 */
		virtual GraphicsPort* newInternalGraphicsPort(Rect clipRect);

		/**
		 * Get the index of the next visible gadget higher up the z-order.
		 * @param startIndex The starting index.
		 * @return The index of the next highest visible gadget.
		 */
		const s32 getHigherVisibleGadget(const s32 startIndex) const;

		/**
		 * Get the index of the next visible gadget lower down the z-order.
		 * @param startIndex The starting index.
		 * @return The index of the next lowest visible gadget.
		 */
		const s32 getLowerVisibleGadget(const s32 startIndex) const;

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Gadget(const Gadget& gadget) { };
	};
}

#endif
