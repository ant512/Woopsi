#ifndef _WOOPSI_H_
#define _WOOPSI_H_

#include <nds.h>
#include <vector>
#include "gadget.h"

using namespace std;

class Screen;
class ContextMenu;

/**
 * Class providing a top-level gadget and an interface to the Woopsi gadget hierarchy.
 */
class Woopsi : public Gadget {

public:
	/**
	 * The singleton provides an instance of Woopsi that should be instantiated and
	 * used as the only Woopsi instance.  The Debug class in particular interacts
	 * with this singleton.
	 */
	static Woopsi *singleton;
	
	/**
	 * Constructor.
	 * @param font Pointer to a font object for use within this class.
	 */
	Woopsi(FontBase* font = NULL);
	
	/**
	 * Destructor.
	 */
	virtual ~Woopsi();

	/**
	 * Add a new screen to the Woopsi instance.
	 * This should be removed.
	 * @param title The title of the new screen.
	 */
	virtual Screen* newScreen(char* title);
	
	/**
	 * Run the Woopsi instace.
	 * This should be called every VBL in order for Woopsi to work.
	 */
	virtual void play();
	
	/**
	 * Draw the entire Woopsi GUI to the display.
	 * Should be called before the main loop runs.
	 */
	virtual inline void draw() { Gadget::draw(); };
	
	/**
	 * Draw a specific rectangle of the GUI.
	 * The supplied region will be drawn with no further clipping checks.
	 * For use only within Woopsi gadgets.
	 * @param clipRect The region to draw.
	 */
	virtual void draw(Rect clipRect);
	
	/**
	 * Process stylus events and send throughout the hierarchy.
	 */
	virtual void handleStylus();

	/**
	 * Process keypad events and send throughout the hierarchy.
	 */
	virtual void handleKeys();

	/**
	 * Process lid events and send throughout the hierarchy.
	 */
	virtual void handleLid();

	/**
	 * Receive stylus clicks and process them.
	 * @param x The x co-ordinate of the click.
	 * @param y The y co-ordinate of the click.
	 * @return True if clicked.
	 */
	virtual bool click(s16 x, s16 y);

	/**
	 * Receive stylus shift clicks and process them.
	 * @param x The x co-ordinate of the click.
	 * @param y The y co-ordinate of the click.
	 * @return True if clicked.
	 */
	virtual bool shiftClick(s16 x, s16 y);

	/**
	 * Receive stylus releases and process them.
	 * @param x The x co-ordinate of the release.
	 * @param y The y co-ordinate of the release.
	 * @return True if released.
	 */
	virtual bool release(s16 x, s16 y);

	/**
	 * Receive stylus drags and process them.
	 * @param x The current x co-ordinate of the stylus.
	 * @param y The current y co-ordinate of the stylus.
	 * @param vX The horizontal distance dragged.
	 * @param vY The vertical distance dragged.
	 * @return True if dragged.
	 */
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	/**
	 * Receive and process VBLs.
	 * @return True if VBL was processed.
	 */
	virtual bool vbl();

	/**
	 * Sets the active gadget pointer to the supplied gadget.
	 * @param gadget The gadget to point the internal pointer at; must be a child of this gadget.
	 */
	virtual void setActiveGadget(Gadget* gadget);

	/**
	 * Swaps the depth of the supplied gadget.
	 * This function presumes that all child gadgets are screens.
	 * @param gadget The gadget to be depth-swapped.
	 * @return True if the depth swap occurred.
	 */
	virtual bool swapGadgetDepth(Gadget* gadget);

	/**
	 * Flips the supplied gadget from its current screen to the other screen.
	 * This functions presumes that all child gadgets are screens.
	 * @param gadget The gadget (must be a screen) to flip.
	 * @return True if the flip occurred.
	 */
	virtual bool flipScreens(Gadget* gadget);

	/**
	 * Redraws any dirty regions within the supplied region.
	 * @param rect The region to redraw
	 */
	virtual void eraseRect(Rect rect);

	/**
	 * Get a pointer to the system font.
	 * @return A pointer to the system font.
	 */
	static FontBase* getSystemFont();

	/**
	 * Deletes the system font.
	 * Note that this should not be called.
	 */
	static void deleteSystemFont();

	/**
	 * Add the supplied gadget to the list of gadgets that receive VBL notifications.
	 * Gadgets not in this list will not be able to update automatically when the screen refreshes.
	 * @param A pointer to the gadget to add to the VBL list.
	 */
	static void registerForVBL(Gadget* gadget);

	/**
	 * Remove the supplied gadget from the list of gadgets that receive VBL notifications.
	 * Gadgets removed from the list will no longer be able to update automatically when the screen refreshes.
	 * @param gadget A pointer to the gadget to remove from the VBL list.
	 */
	static void unregisterFromVBL(Gadget* gadget);

	/**
	 * Add a gadget to the list of gadgets to be deleted.
	 * Must never be called by anything other than the framework itself.
	 * @param gadget The gadget to add to the delete queue.
	 */
	static void addToDeleteQueue(Gadget* gadget);

	/**
	 * Return the number of VBLs that have occurred since Woopsi began running.
	 * The count will eventually overflow the 32-bit int and reset to 0.  Developers must allow for this.
	 * @return The VBL count.
	 */
	static u32 getVBLCount();

	/**
	 * Get a pointer to the context menu.
	 * @return Pointer to the context menu.
	 */
	inline ContextMenu* getContextMenu() { return _contextMenu; };
	
	void showContextMenu();

protected:
	bool _lidClosed;
	
	static DynamicArray<Gadget*> _vblListeners;
	static DynamicArray<Gadget*> _deleteQueue;
	static FontBase* _systemFont;
	static u32 _vblCount;
	ContextMenu* _contextMenu;

	/**
	 * Closes a child gadget.
	 * @param gadget The child gadget to close.
	 */
	virtual void closeChild(Gadget* gadget);
	
	/**
	 * Hides a child gadget.
	 * @param gadget The child gadget to hide.
	 */
	virtual void hideChild(Gadget* gadget);

	/**
	 * Delete any gadgets in the deletion queue.
	 */
	void processDeleteQueue();
};

#define woopsiApplication (Woopsi::singleton)
#endif
