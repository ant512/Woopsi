#ifndef _GADGET_H_
#define _GADGET_H_

#include <nds.h>
#include "defines.h"
#include "fontbase.h"
#include "gadgeteventhandler.h"
#include "gadgetstyle.h"
#include "glyphs.h"
#include "rect.h"
#include "woopsiarray.h"
#include "woopsistring.h"

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
			u8 enabled : 1;						/**< True if the gadget is enabled. */
			u8 decoration : 1;					/**< True if the gadget is a decoration. */
			u8 permeable : 1;					/**< True if the gadget's children can exceed its dimensions. */
			u8 erased : 1;						/**< True if the gadget is currently erased from the frame buffer. */
			u8 hidden : 1;						/**< True if the gadget is hidden. */
			u8 doubleClickable : 1;				/**< True if the gadget can be double-clicked. */
			u8 modal : 1;						/**< True if the gadget is modal. */
			u8 canReceiveFocus : 1;				/**< True if the gadget can receive focus. */
			u8 raisesEvents : 1;				/**< True if the gadget raises events to its handler. */
		} Flags;

		/**
		 * Struct describing a name/value pair.
		 */
		typedef struct {
			u32 value;							/**< Value associated with the name. */
			WoopsiString name;					/**< Name associated with the value. */
		} NameValuePair;

		/**
		 * Struct describing the size of all four borders of a gadget.
		 */
		typedef struct {
			u8 top;								/**< Height of the top border. */
			u8 right;							/**< Width of the right border. */
			u8 bottom;							/**< Height of the bottom border. */
			u8 left;							/**< Width of the left border. */
		} GadgetBorderSize;

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 * data.  If no object is specified the gadget will use the default style.
		 * @see GadgetFlagType.
		 */
		Gadget(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style = NULL);
		
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
		 * Get the x co-ordinate of the gadget relative to its parent.
		 * @return Parent-space x co-ordinate.
		 */
		const s16 getRelativeX() const;

		/**
		 * Get the y co-ordinate of the gadget relative to its parent.
		 * @return Parent-space y co-ordinate.
		 */
		const s16 getRelativeY() const;

		/**
		 * Get the reference constant for this gadget.
		 * @return The reference constant.
		 */
		inline const u32 getRefcon() const { return _refcon; };

		/**
		 * Is the gadget active?
		 * A value of true indicates that this gadget has focus or is an
		 * ancestor of the gadget with focus.
		 * @return True if active.
		 */
		inline const bool hasFocus() const { return _flags.hasFocus; };

		/**
		 * Has the gadget been marked for deletion?  This function recurses up
		 * the gadget hierarchy and only returns true if all of the gadgets in
		 * the ancestor chain are not deleted.  
		 * Gadgets marked for deletion are automatically deleted and should not
		 * be interacted with.
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
		 * Decoration gadgets are children of, but also an essential component
		 * of, another gadget.  They are always at the start of the child
		 * gadget array, so will always be at the bottom of the gadget stack.
		 * They can be thought of as background gadgets.
		 * @return True if the gadget is a decoration.
		 */
		inline const bool isDecoration() const { return _flags.decoration; };

		/**
		 * Are the gadget's edges permeable or solid?
		 * Permeable gadgets do not enforce their dimensions on the
		 * co-ordinates and dimensions of child gadgets.  For example, windows
		 * belonging to non-permeable screens cannot be dragged outside the
		 * screen.  Windows belonging to permeable screens *can* be dragged
		 * outside their parent screen.
		 * @return True if permeable.
		 */
		inline const bool isPermeable() const { return _flags.permeable; };

		/**
		 * Is the gadget double-clickable?  If this is false, double-clicks will
		 * be detected and processed as single-clicks.
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
		 * Is the gadget being dragged?
		 * @return True if the gadget is currently being dragged.
		 */
		inline const bool isBeingDragged() const { return _flags.dragging; };

		/**
		 * Is the gadget shelved?
		 * @return True if the gadget is shelved.
		 */
		inline const bool isShelved() const { return _flags.shelved; };

		/**
		 * Is the gadget modal?  Only true if the Woopsi singleton is also
		 * modal.
		 * @return True if the gadget is modal.
		 */
		const bool isModal() const;
		
		/**
		 * Check if the gadget can receive focus or not.
		 * @return True if the gadget can receive focus.
		 */
		const bool canReceiveFocus() const;

		/**
		 * Get the width of the gadget.
		 * @return The gadget width.
		 */
		inline u16 getWidth() const { return _rect.getWidth(); };

		/**
		 * Get the height of the gadget.
		 * @return The gadget height.
		 */
		inline u16 getHeight() const { return _rect.getHeight(); };

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
		 * Get the number of the screen that this gadget is currently displayed
		 * on.
		 * @return 0 for the touch screen, 1 for the top screen.
		 */
		virtual const u8 getPhysicalScreenNumber() const;

		/**
		 * Check if this gadget raises events or not.
		 * @return True if events are enabled and a handler is set.
		 */
		inline const bool raisesEvents() const { return _gadgetEventHandler && _flags.raisesEvents && !_flags.shelved; };

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the
		 * gadget's parent.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

		/**
		 * Insert the properties of the space within this gadget that is
		 * available for children into the rect passed in as a parameter.
		 * All co-ordinates are relative to this gadget.
		 * @param rect Reference to a rect to populate with data.
		 */
		void getClientRect(Rect& rect) const;

		/**
		 * Clips the supplied rect to the boundaries defined by this gadget and
		 * this gadget's parents.  Co-ordinates are in Woopsi-space.
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
		GraphicsPort* newGraphicsPort(bool isForeground);

		/**
		 * Gets a pointer to a new instance of the GraphicsPort class to allow
		 * drawing within this gadget's client space.  The GraphicsPort can
		 * only draw within the supplied region.
		 *
		 * The GraphicsPort object must be deleted when it is no longer
		 * required.  Drawing is clipped only to the supplied rect.  If the rect
		 * is not within the visible regions of this gadget graphical anomalies
		 * will occur.  This should only be called by the Woopsi hierarchy.
		 * @param clipRect The region to clip to.  Co-ordinates are in Woopsi-
		 * space.
		 * @return A pointer to a new GraphicsPort object.
		 */
		GraphicsPort* newGraphicsPort(Rect clipRect);

		/**
		 * Gets a pointer to the vector of all of the visible regions of this
		 * gadget, including any covered by children.
		 * @return A pointer to a vector of all visible regions.
		 */
		WoopsiArray<Rect>* getForegroundRegions();

		/**
		 * Gets a pointer to the gadget's font.
		 * @return A pointer to the gadget's font.
		 */
		FontBase* getFont() const;
		
		/**
		 * Gets a pointer to the gadget's glyph font.
		 * @return A pointer to the gadget's glyph font.
		 */
		FontBase* getGlyphFont() const;

		/**
		 * Gets the colour used as the background fill.
		 * @return Background fill colour.
		 */
		inline const u16 getBackColour() const { return _style.colours.back; };

		/**
		 * Gets the colour used as the light edge in bevelled boxes.
		 * @return Shine colour.
		 */
		inline const u16 getShineColour() const { return _style.colours.shine; };

		/**
		 * Gets the colour used as the fill in focused window borders.
		 * @return Highlight colour.
		 */
		inline const u16 getHighlightColour() const { return _style.colours.highlight; };

		/**
		 * Gets the colour used as the dark edge in bevelled boxes.
		 * @return Shadow colour.
		 */
		inline const u16 getShadowColour() const { return _style.colours.shadow; };

		/**
		 * Gets the colour used as the fill in unfocused window borders.
		 * @return Fill colour.
		 */
		inline const u16 getFillColour() const { return _style.colours.fill; };

		/**
		 * Gets the colour used as the fill in scrollbar gutters.
		 * @return Dark colour.
		 */
		inline const u16 getDarkColour() const { return _style.colours.dark; };

		/**
		 * Gets the colour used for text.
		 * @return Text colour.  0 indicates the font default will be used. 
		 */
		inline const u16 getTextColour() const { return _style.colours.text; };

		/**
		 * Sets this gadget's reference constant.  This should be unique, at
		 * least amongst this gadget's siblings.
		 * @param refcon The reference constant.
		 */
		u32 setRefcon(u32 refcon);

		/**
		 * Sets this gadget's border state.
		 * @param isBorderless The border state.
		 */
		virtual void setBorderless(bool isBorderless);

		/**
		* Sets this gadget as a decoration or otherwise.
		 * @param isDecoration The gadget's decoration state.
		 */
		void setDecoration(bool isDecoration);

		/**
		 * Sets whether or not this gadget can be dragged.
		 * @param isDraggable The draggable state.
		 */
		inline void setDraggable(const bool isDraggable) { _flags.draggable = isDraggable; };

		/**
		 * Sets whether or not child gadgets can exceed this gadget's
		 * dimensions.
		 * @param isPermeable The permeable state.
		 */
		inline void setPermeable(const bool isPermeable) { _flags.permeable = isPermeable; };

		/**
		 * Sets whether or not the gadgets processes double-clicks.
		 * @param isDoubleClickable The double-clickable state.
		 */
		inline void setDoubleClickable(const bool isDoubleClickable) { _flags.doubleClickable = isDoubleClickable; };

		/**
		 * Sets the gadget event handler.  The event handler will receive
		 * all events raised by this gadget.
		 * @param eventHandler A pointer to the event handler.
		 */
		inline void setGadgetEventHandler(GadgetEventHandler* eventHandler) { _gadgetEventHandler = eventHandler; };

		/**
		 * Gets the gadget event handler.
		 * @return A pointer to the event handler.
		 */
		inline GadgetEventHandler* getGadgetEventHandler() { return _gadgetEventHandler; };
		/**
		 * Enables or disables event firing for this gadget.
		 * @param raisesEvents True to enable events, false to disable.
		 */
		inline void setRaisesEvents(const bool raisesEvents) { _flags.raisesEvents = raisesEvents; };

		/**
		 * Sets the background colour.
		 * @param colour The new background colour.
		 */
		inline void setBackColour(const u16 colour) { _style.colours.back = colour; };

		/**
		 * Sets the shine colour.
		 * @param colour The new shine colour.
		 */
		inline void setShineColour(const u16 colour) { _style.colours.shine = colour; };

		/**
		 * Sets the highlight colour.
		 * @param colour The new highlight colour.
		 */
		inline void setHighlightColour(const u16 colour) { _style.colours.highlight = colour; };

		/**
		 * Sets the shadow colour.
		 * @param colour The new shadow colour.
		 */
		inline void setShadowColour(const u16 colour) { _style.colours.shadow = colour; };

		/**
		 * Sets the fill colour.
		 * @param colour The new fill colour.
		 */
		inline void setFillColour(const u16 colour) { _style.colours.fill = colour; };

		/**
		 * Sets the dark colour.
		 * @param colour The new dark colour.
		 */
		inline void setDarkColour(const u16 colour) { _style.colours.dark = colour; };

		/**
		 * Sets the text colour.
		 * @param colour The new text colour.  0 specifies the font default.
		 */
		inline void setTextColour(const u16 colour) { _style.colours.text = colour; };

		/**
		 * Sets the font.
		 * @param font A pointer to the font to use.
		 */
		virtual void setFont(FontBase* font);
		
		/**
		 * Sets the glyph font.
		 * @param font A pointer to the font to use.
		 */
		virtual void setGlyphFont(FontBase* font);

		/**
		 * Redraws the region of the gadget represented by rect.  Rect should
		 * be supplied in Woopsi-space co-ordinates and should be pre-clipped
		 * to the visible regions of the gadget.  This function should only
		 * ever be called by the DisplayController.
		 * @param rect The rect to draw.
		 */
		void redraw(const Rect& rect);

		/**
		 * Enables the gadget.
		 * @return True if the gadget was enabled.
		 * @see disable()
		 */
		bool enable();

		/**
		 * Disabled the gadget.
		 * @return True if the gadget was disabled.
		 * @see enable()
		 */
		bool disable();

		/**
		 * Erases the gadget, marks it as deleted, and moves it to Woopsi's
		 * deletion queue.  Gadgets are automatically deleted by the framework
		 * and should not be deleted externally.
		 */
		void close();

		/**
		 * Erases the gadget, removes it from the main hierarchy and sets it to
		 * invisible.  Gadgets hidden in this way will be partioned off from
		 * other gadgets and will no longer be processed.
		 * @return True if the gadget was shelved.
		 * @see unshelve()
		 */
		bool shelve();

		/**
		 * Moves the gadget back into the hierarchy and marks it for redraw.
		 * Gadgets shown in this way will be unpartioned and will be processed
		 * again.
		 * @return True if the gadget was unshelved.
		 * @see shelve()
		 */
		bool unshelve();

		/**
		 * Draws the gadget and makes it visible.
		 * Does not steal focus from other gadgets.
		 * @return True if the gadget was shown.
		 * @see hide()
		 */
		bool show();

		/**
		 * Erases the gadget and makes it invisible.
		 * Does not re-assign focus to another gadget.
		 * @return True if the gadget was hidden.
		 * @see show()
		 */
		bool hide();

		/**
		 * Click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		bool click(s16 x, s16 y);

		/**
		 * Check if the click is a double-click.
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
		bool doubleClick(s16 x, s16 y);

		/**
		 * Shift-click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		bool shiftClick(s16 x, s16 y);

		/**
		 * Release this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the release.
		 * @param y Y co-ordinate of the release.
		 * @return True if the release was successful.
		 */
		bool release(s16 x, s16 y);

		/**
		 * Drag the gadget to the supplied co-ordinates.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 * @param vX The horizontal distance that the stylus was dragged.
		 * @param vY The vertical distance that the stylus was dragged.
		 * @return True if the drag was successful.
		 */
		bool drag(s16 x, s16 y, s16 vX, s16 vY);

		/**
		 * Send a keypress to the gadget.
		 * @param keyCode The keycode to send to the gadget.
		 * @return True if the keypress was processed.
		 */
		bool keyPress(KeyCode keyCode);
		
		/**
		 * Send a key repeat to the gadget.
		 * @param keyCode The keycode to send to the gadget.
		 * @return True if the key repeat was processed.
		 */
		bool keyRepeat(KeyCode keyCode);

		/**
		 * Send a key release to the gadget.
		 * @param keyCode The keycode to send to the gadget.
		 * @return True if the key release was processed.
		 */
		bool keyRelease(KeyCode keyCode);

		/**
		 * Inform the gadget that the lid has closed.
		 * @see lidOpened()
		 */
		void lidClose();

		/**
		 * Inform the gadget that the lid has opened.
		 * @see lidClosed()
		 */
		void lidOpen();

		/**
		 * Give the gadget focus.
		 * @return True if the gadget received focus correctly.
		 * @see blur()
		 */
		bool focus();

		/**
		 * Remove focus from the gadget.
		 * @return True if the gadget lost focus correctly.
		 * @see focus()
		 */
		bool blur();

		/**
		 * Move the gadget to the new co-ordinates.
		 * Co-ordinates are relative to the parent gadget.
		 * @param x The new x co-ordinate.
		 * @param y The new y co-ordinate.
		 * @return True if the move was successful.
		 */
		bool moveTo(s16 x, s16 y);

		/**
		 * Resize the gadget to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		bool resize(u16 width, u16 height);

		/**
		 * Resize and move the gadget in one operation.
		 * @param x The new x co-ordinate.
		 * @param y The new y co-ordinate.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the gadget was adjusted successfully.
		 */
		bool changeDimensions(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Raises the gadget to the top of its parent's gadget stack.
		 * @return True if the raise was successful.
		 */
		bool raiseToTop();

		/**
		 * Lowers the gadget to the bottom of its parent's gadget stack.
		 * @return True if the lower was successful.
		 */
		bool lowerToBottom();

		/**
		 * Raises the supplied gadget to the top of this gadget's child stack.
		 * The supplied gadget pointer must be a child of this gadget.
		 * @param gadget A pointer to the child gadget to raise.
		 * @return True if the raise was successful.
		 */
		bool raiseGadgetToTop(Gadget* gadget);

		/**
		 * Lowers the supplied gadget to the bottom of this gadget's child
		 * stack.
		 * The supplied gadget pointer must be a child of this gadget.
		 * @param gadget A pointer to the child gadget to lower.
		 * @return True if the lower was successful.
		 */
		bool lowerGadgetToBottom(Gadget* gadget);

		/**
		 * Moves the supplied child gadget to the deletion queue.
		 * For framework use only.
		 * @param gadget A pointer to the child gadget.
		 */
		void moveGadgetToDeleteQueue(Gadget* gadget);

		/**
		 * Moves the supplied child gadget to the shelved gadget list.
		 * For framework use only.
		 * @param gadget A pointer to the child gadget.
		 * @return True if the gadget was moved successfully.
		 * @see moveShelvedToGadgetList()
		 * @see hide()
		 */
		bool moveGadgetToShelvedList(Gadget* gadget);

		/**
		 * Moves the supplied child gadget from the shelved list back
		 * to the child gadget list.
		 * For framework use only.
		 * @param gadget A pointer to the shelved gadget.
		 * @return True if the gadget was moved successfully.
		 * @see moveGadgetToShelvedList()
		 * @see show()
		 */
		bool moveShelvedToGadgetList(Gadget* gadget);

		/**
		 * Sets the supplied gadget as the focused child.  The gadget must
		 * be a child of this gadget.
		 * @param gadget A pointer to the child gadget.
		 * @see getFocusedGadget()
		 */
		void setFocusedGadget(Gadget* gadget);

		/**
		 * Checks if the supplied point collide with this gadget.
		 * Co-ordinates must be in Woopsi-space.
		 * @param point The point to check.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(const WoopsiPoint& point) const;

		/**
		 * Checks if the supplied co-ordinates collide with this gadget.
		 * Co-ordinates must be in Woopsi-space.
		 * @param x The x co-ordinate to check.
		 * @param y The y co-ordinate to check.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(s16 x, s16 y) const;

		/**
		 * Checks if the supplied rectangle definition collides with this
		 * gadget.  Co-ordinates must be in Woopsi-space.
		 * @param x The x co-ordinate of the rectangle to check.
		 * @param y The y co-ordinate of the rectangle to check.
		 * @param width The width of the rectangle to check.
		 * @param height The height of the rectangle to check.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(s16 x, s16 y, u16 width, u16 height) const;

		/**
		 * Checks if the supplied gadget collides with this gadget.
		 * @param gadget A pointer to another gadget to check for collisions
		 * with.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(const Gadget* gadget) const;

		/**
		 * Checks if the supplied rect collides with this gadget.  Co-ordinates
		 * must be in Woopsi-space.
		 * @param rect A rect to check for collisions with.
		 * @return True if a collision occurred.
		 */
		bool checkCollision(const Rect& rect) const;

		/**
		 * Invalidate the visible region cache for all gadgets below the
		 * supplied gadget in this gadget's child stack.  This will cause those
		 * gadgets to recalculate their visible regions next time they try to
		 * draw themselves.
		 * This should be called when the gadget passed as a pointer has swapped
		 * positions with another gadget in the gadget array (ie. it has moved
		 * towards the front of the screen or towards the back).
		 * @param gadget A pointer to a child gadget.
		 */
		void invalidateLowerGadgetsVisibleRectCache(Gadget* gadget);

		/**
		 * Adds a gadget to this gadget's child stack.  The gadget is added to
		 * the top of the stack.  Note that the gadget can only be added if it
		 * is not already a child of another gadget.
		 *
		 * If the gadget is a decoration, the gadget will be inserted to the
		 * bottom of the stack instead of the top.  Decorations are always at
		 * the bottom of the gadget stack.  This is automatic and cannot be
		 * overridden.
		 *
		 * Once a gadget has been added to another, it becomes the
		 * responsibility of Woopsi to delete it once it it no longer needed.
		 * Attempts to manually call "delete" on child gadgets will result in
		 * crashes due to dereferencing deleted objects.
		 * @param gadget A pointer to the gadget to add to the child list.
		 * @see insertGadget()
		 * @see remove()
		 */
		void addGadget(Gadget* gadget);

		/**
		 * Inserts a gadget into this gadget's child stack at the bottom of the
		 * stack.  Note that the gadget can only be added if it is not already
		 * a child of another gadget.
		 *
		 * Once a gadget has been added to another, it becomes the
		 * responsibility of Woopsi to delete it once it it no longer needed.
		 * Attempts to manually call "delete" on child gadgets will result in
		 * crashes due to dereferencing deleted objects.
		 * @param gadget A pointer to the gadget to add to the child list.
		 * @see addGadget()
		 * @see remove()
		 */
		void insertGadget(Gadget* gadget);

		/**
		 * Set the gadget's parent to the gadget passed in as a parameter.
		 * Called automatically when a gadget is added as a child.  It shouldn't
		 * be called in user code.
		 * @param parent A pointer to the parent gadget.
		 */
		inline void setParent(Gadget* parent) { _parent = parent; };

		/**
		 * Rebuild the list of this gadget's visible regions.  If the cache is
		 * already up to date this function will just exit.
		 */
		void cacheVisibleRects() const;

		/**
		 * Mark this gadget's visible region cache as invalid, and do the same
		 * to its child gadgets.
		 */
		void invalidateVisibleRectCache();

		/**
		 * Clips a rectangular region to the dimensions of this gadget and its
		 * ancestors.
		 * @param rect The region that needs to be clipped.
		 */
		void clipRectToHierarchy(Rect& rect) const;

		/**
		 * Moves the the supplied child gadget from its current location in the
		 * child list to the new location.
		 * @param sourceIndex The current index of the child gadget.
		 * @param destinationIndex The new index for the child gadget.
		 * @return True if the swap was successful.
		 */
		bool changeGadgetDepth(s32 sourceIndex, s32 destinationIndex);

		/**
		 * Gets the lowest non-decoration gadget in the child list that collides
		 * with the specified gadget.
		 * @param gadget The gadget to check for collisions.
		 * @return The index of the lowest colliding gadget, or -1 if no gadgets
		 * collide with the specified gadget.
		 */
		s32 getLowestCollidingGadgetIndex(const Gadget* gadget) const;

		/**
		 * Gets the highest non-decoration gadget in the child list that
		 * collides with the specified gadget.
		 * @param gadget The gadget to check for collisions.
		 * @return The index of the highest colliding gadget, or -1 if no
		 * gadgets collide with the specified gadget.
		 */
		s32 getHighestCollidingGadgetIndex(const Gadget* gadget) const;

		/**
		 * Swap the depth of this gadget.  Compares the gadget's depth with all
		 * non-decoration sibling gadgets that it overlaps.  If it is not at the
		 * front of the overlapping pile, it is moved to the front.  If it is at
		 * the front of the overlapping pile, it moves the gadget to the back of
		 * the pile.
		 * @return True if the swap was successful.
		 * @see swapGadgetDepth()
		 */
		virtual bool swapDepth();

		/**
		 * Delete this gadget.  This should never be called in user code; gadget
		 * deletion is handled by Woopsi.
		 */
		inline void destroy() { delete this; };

		/**
		 * Remove this gadget from Woopsi's gadget hierarchy.  Returns
		 * responsibility for deleting the gadget back to the developer.
		 * Does not unregister the gadget from the VBL system.
		 * @return True if the gadget was successfully removed.
		 * @see removeGadget()
		 */
		bool remove();

		/**
		 * Remove a child gadget from Woopsi's gadget hierarchy.  Returns
		 * responsibility for deleting the gadget back to the developer.
		 * Does not unregister the gadget from the VBL system.
		 * @param gadget Pointer to the gadget to remove from the hierarchy.
		 * @return True if the gadget was succesfully removed.
		 * @see remove()
		 */
		bool removeGadget(Gadget* gadget);

		/**
		 * Add a context menu item definition to the gadget.
		 * @param name The name of the menu item.
		 * @param value The value of the menu item.
		 */
		void addContextMenuItem(const WoopsiString& name, u32 value);

		/**
		 * Show the context menu for this gadget at the specified co-ordinates.
		 * @param x The x co-ordinate of the context menu, relative to the
		 * topmost screen on the bottom display.
		 * @param y The y co-ordinate of the context menu, relative to the
		 * topmost screen on the bottom display.
		 */
		void showContextMenu(s16 x, s16 y);

		/**
		 * Raises the context menu selection event to the gadget's event
		 * handlers.
		 * @param item The selected menu item.
		 */
		virtual bool handleContextMenuSelection(const ListDataItem* item);

		/**
		 * Run the gadget modally.  All other gadgets will become non-responsive
		 * and only this gadget will be interactive.  Useful for requesters that
		 * must be dealt with before the application can continue.
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
		Gadget* getGadget(u32 index) const;

		/**
		 * Get the quantity of child gadgets, including decoration gadgets.
		 * The quantity of child gadgets not including decorations can be found
		 * by calling getChildCount() - getDecorationCount().
		 * @return The number of child gadgets belonging to this gadget.
		 * @see getDecorationCount()
		 */
		const s32 getGadgetCount() const { return _gadgets.size(); };

		/**
		 * Get the quantity of decoration gadgets.
		 * @return The number of decoration gadgets belonging to this gadget.
		 * @see getGadgetCount()
		 */
		const inline s32 getDecorationCount() const { return _decorationCount; };

		/**
		 * Get a pointer to the cache of visible rects.
		 * @return A pointer to the cache of visible rects.
		 */
		inline RectCache* getRectCache() const { return _rectCache; };

		/**
		 * Sets the border size.  The border cannot be drawn over in the
		 * drawContents() method.
		 * @param borderSize The new border size.
		 */
		void setBorderSize(const GadgetBorderSize& borderSize);

		/**
		 * Marks all visible portions of the gadget for redrawing.
		 * Should be called when a gadget's appearance has changed.
		 */
		void markRectsDamaged();
		
		/**
		 * Marks a particular rect for drawing.  Can be called instead of
		 * markRectsDamaged() if only a portion of a gadget has changed.
		 * The rect is automatically clipped to the visible portions of the
		 * gadget.  Its co-ordinates should be in gadget co-ordinates (ie.
		 * relative to the top-left corner of the gadget, which is (0,0)).
		 * It will automatically be converted to Woopsi co-ordinates.
		 * @param rect Rect to queue for redraw.
		 */
		void markRectDamaged(const Rect& rect);

	protected:
		Rect _rect;								/**< Rect describing the gadget. */
		u32 _refcon;							/**< Identifying number of the gadget. */

		// Dragging variables
		s16 _grabPointX;						/**< Physical space x co-ordinate where dragging began. */
		s16 _grabPointY;						/**< Physical space y co-ordinate where dragging began. */
		s16 _newX;								/**< Physical x co-ordinate where gadget is being dragged to. */
		s16 _newY;								/**< Physical y co-ordinate where gadget is being dragged to. */

		// Style
		GadgetStyle _style;						/**< All style information used by a gadget. */

		// Status
		Flags _flags;							/**< Flags struct. */

		// Event handling
		GadgetEventHandler* _gadgetEventHandler;/**< Delegate that responds to gadget events. */

		// Double-clicking
		u32 _lastClickTime;						/**< VBL count when last clicked. */
		s16 _lastClickX;						/**< X co-ordinate of last click. */
		s16 _lastClickY;						/**< Y co-ordinate of last click. */
		s16 _doubleClickBounds;					/**< Area in which a click is assumed to be a double-click. */

		// Hierarchy control
		Gadget* _parent;						/**< Pointer to the gadget's parent. */
		Gadget* _focusedGadget;					/**< Pointer to the child gadget that has focus. */
		WoopsiArray<Gadget*> _gadgets;			/**< List of child gadgets. */
		WoopsiArray<Gadget*> _shelvedGadgets;	/**< List of shelved child gadgets. */

		// Decorations
		s32 _decorationCount;					/**< Total number of decoration child gadgets. */

		// Visible regions
		RectCache* _rectCache;					/**< List of the gadget's visible regions. */

		GadgetBorderSize _borderSize;			/**< Size of the gadget borders. */

		// Context menu item definitions
		WoopsiArray<NameValuePair> _contextMenuItems;	/**< List of all context menu name/value pairs. */

		/**
		 * Destructor.
		 */
		virtual ~Gadget();

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * @param port The GraphicsPort to draw to.
		 */
		virtual inline void drawContents(GraphicsPort* port) { };

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * @param port The GraphicsPort to draw to.
		 */
		virtual void drawBorder(GraphicsPort* port) { };

		/**
		 * Checks if the supplied co-ordinates collide with a portion of this
		 * gadget that is not obscured by its siblings or ancestors, but that
		 * may be obscured by its children.  Co-ordinates should be in
		 * Woopsi-space.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if a collision occurred; false if not.
		 */
		bool checkCollisionWithForegroundRects(s16 x, s16 y) const;

		/**
		 * Get the current physical display co-ordinate for the supplied y
		 * co-ordinate.  Woopsi treats the two displays as two viewports on the
		 * same logical space.  The lower half of the range of y co-ordinates is
		 * displayed on the bottom screen, whilst the upper half of the range of
		 * y co-ordinates is displayed on the top screen.  This function
		 * converts back into a value that can be displayed on one of the
		 * screens.  Should be used in conjunction with
		 * calculatePhysicalScreenNumber() to work out which screen to draw on.
		 * @param y The y co-ordinate to check.
		 * @return The converted y co-ordinate.
		 * @see calculatePhysicalScreenNumber()
		 */
		const s16 calculatePhysicalScreenY(s16 y) const;

		/**
		 * Get the current physical display number for the supplied y
		 * co-ordinate.  Should be used in conjunction with
		 * calculatePhysicalScreenY() to convert to a y co-ordinate that can be
		 * displayed.
		 * @param y The y co-ordinate to check.
		 * @return 0 if the co-ordinate falls within the bottom screen; 1 if it
		 * falls within the top screen.
		 * @see calculatePhysicalScreenY()
		 */
		const u8 calculatePhysicalScreenNumber(s16 y) const;

		/**
		 * Erase and remove the supplied child gadget from this gadget and send
		 * it to the deletion queue.
		 * @param gadget The gadget to close.
		 * @see close().
		 */
		void closeGadget(Gadget* gadget);

		/**
		 * Erase the supplied child gadget and move it out of the main child
		 * list into the shelved list.  The gadget remains in memory and can
		 * be restored by calling "unshelve()" on the gadget.
		 * @param gadget The gadget to hide.
		 * @see shelve()
		 * @see unshelve()
		 * @see shelveGadget()
		 */
		void shelveGadget(Gadget* gadget);

		/**
		 * Get a graphics port that can draw within the region of the supplied
		 * clipping rect.  The port must be deleted when it is no longer
		 * required.
		 *
		 * Note that the clipping rect should be clipped to the gadget's visible
		 * region before creating the graphics port.  The rect should be in
		 * Woopsi-space co-ordinates.
		 * @return A new graphics port object.
		 */
		GraphicsPort* newInternalGraphicsPort(Rect clipRect);

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
		 * Notify this gadget that it is being dragged, and set its drag point.
		 * @param x The x co-ordinate of the drag position relative to this
		 * gadget.
		 * @param y The y co-ordinate of the drag position relative to this
		 * gadget.
		 */
		void startDragging(s16 x, s16 y);
		
		/**
		 * Notify this gadget that it is no longer being dragged.
		 * @param x The x co-ordinate at which dragging stopped.
		 * @param y The y co-ordinate at which dragging stopped.
		 */
		void stopDragging(s16 x, s16 y);

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Gadget(const Gadget& gadget) { };

		/**
		 * Called when the gadget is clicked.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when it is
		 * clicked.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual inline void onClick(s16 x, s16 y) { };
		
		/**
		 * Called when the gadget is double-clicked.  Override this when
		 * creating new gadgets if the gadget should exhibit additional
		 * behaviour when it is double-clicked.  To change the conditions that
		 * apply in detecting a double-click, override the isDoubleClicked()
		 * method.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual inline void onDoubleClick(s16 x, s16 y) { };
		
		/**
		 * Called when the gadget is shift-clicked.  Override this when
		 * creating new gadgets if the gadget should exhibit additional
		 * behaviour when it is shift-clicked.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual inline void onShiftClick(s16 x, s16 y) { };
		
		/**
		 * Called when the gadget is released.  Override this when
		 * creating new gadgets if the gadget should exhibit additional
		 * behaviour when it is released.
		 * @param x The x co-ordinate of the stylus when released.
		 * @param y The y co-ordinate of the stylus when released.
		 */
		virtual inline void onRelease(s16 x, s16 y) { };
		
		/**
		 * Called when the gadget is released outside of its boundaries.
		 * Override this when creating new gadgets if the gadget should exhibit
		 * additional behaviour when it is released outside of its boundaries.
		 * @param x The x co-ordinate of the stylus when released.
		 * @param y The y co-ordinate of the stylus when released.
		 */
		virtual inline void onReleaseOutside(s16 x, s16 y) { };
		
		/**
		 * Called when the gadget is dragged.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when it is
		 * dragged.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 * @param vX X distance dragged.
		 * @param vY Y distance dragged.
		 */
		virtual inline void onDrag(s16 x, s16 y, s16 vX, s16 vY) { };
		
		/**
		 * Called when the gadget starts being dragged.  Override this when
		 * creating new gadgets if the gadget should exhibit additional
		 * behaviour when dragging starts.
		 */
		virtual inline void onDragStart() { };
		
		/**
		 * Called when the gadget stops being dragged.  Override this when
		 * creating new gadgets if the gadget should exhibit additional
		 * behaviour when dragging stops.
		 */
		virtual inline void onDragStop() { };
		
		/**
		 * Called when the a key (d-pad or physical button) is pressed.
		 * Override this when creating new gadgets if the gadget should exhibit
		 * additional behaviour when a key is pressed.
		 * @param keyCode The key that was pressed.
		 */
		virtual inline void onKeyPress(KeyCode keyCode) { };
		
		/**
		 * Called when a key (d-pad or physical button) is released.  Override
		 * this when creating new gadgets if the gadget should exhibit
		 * additional behaviour when a key is released.
		 * @param keyCode The key that was released.
		 */
		virtual inline void onKeyRelease(KeyCode keyCode) { };
		
		/**
		 * Called when a key (d-pad or physical button) is pressed and repeats.
		 * Override this when creating new gadgets if the gadget should exhibit
		 * additional behaviour when a key repeats.
		 * @param keyCode The key that repeated.
		 */
		virtual inline void onKeyRepeat(KeyCode keyCode) { };
		
		/**
		 * Called when the gadget gains focus.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when
		 * gaining focus.
		 */
		virtual inline void onFocus() { };
		
		/**
		 * Called when the gadget loses focus.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when
		 * losing focus.
		 */
		virtual inline void onBlur() { };
		
		/**
		 * Called when the lid is opened.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when the
		 * lid is opened.
		 */
		virtual inline void onLidOpen() { };
		
		/**
		 * Called when the lid is closed.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when the
		 * lid is closed.
		 */
		virtual inline void onLidClose() { };
		
		/**
		 * Called when the gadget is enabled.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when
		 * enabled.
		 */
		virtual inline void onEnable() { };
		
		/**
		 * Called when the gadget is disabled.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when
		 * disabled.
		 */
		virtual inline void onDisable() { };
		
		/**
		 * Called when the gadget is resized.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when
		 * resized.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual inline void onResize(u16 width, u16 height) { };
	};
}

#endif
