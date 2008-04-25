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
	 * Initialise the application.  All initial GUI creation should go into this method.
	 * This method must call enableDrawing() and draw() at some point.
	 */
	virtual inline void startup() { };

	/**
	 * Main loop.
	 */
	virtual void runLoop();

	/**
	 * Shut down the application.  This base method will shut down the SDL system,
	 * so it must be called as the last function if overridden in an SDL application.
	 */
	virtual inline void shutdown() {
#ifdef _SDL_H_
		SDL_Quit();
#endif
	};

	/**
	 * Run the Woopsi instance.
	 * This should be called every VBL in order for Woopsi to work.
	 * @param gadget Sub-gadget to run, used for modal gadgets; omit
	 * this parameter to run the whole system.
	 */
	virtual void processOneVBL(Gadget* gadget = NULL);
	
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
	 * Add the supplied gadget to the list of gadgets that receive VBL notifications.
	 * Gadgets not in this list will not be able to update automatically when the screen refreshes.
	 * @param gadget A pointer to the gadget to add to the VBL list.
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
	static inline u32 getVBLCount() { return _vblCount; };

	/**
	 * Get a pointer to the context menu.
	 * @return Pointer to the context menu.
	 */
	inline ContextMenu* getContextMenu() { return _contextMenu; };

	/**
	 * Get the current value of the context menu, as determined
	 * by the last selection.
	 * @return The context menu value.
	 */
	const u32 getContextMenuValue() const;

	/**
	 * Shelve the context menu.
	 */
	void shelveContextMenu();

	/**
	 * Aborts the main runloop, allowing other code to take over.
	 */
	inline void abortRunLoop() { _running = false; };

	/**
	 * Main entry point for a Woopsi application.
	 */
	virtual inline int main(int argc, char* argv[]) {
		startup();				// Run any setup code
		runLoop();				// Run the main Woopsi loop
		shutdown();				// Run any shutdown code

		return 0;
	}

protected:
	bool _lidClosed;									/**< Remembers the current state of the lid */
	
	static DynamicArray<Gadget*> _vblListeners;			/**< Array of gadgets that receive VBL events */
	static DynamicArray<Gadget*> _deleteQueue;			/**< Array of gadgets awaiting deletion */
	static FontBase* _systemFont;						/**< Pointer to the default font */
	static u32 _vblCount;								/**< Count of VBLs since Woopsi was first run */
	ContextMenu* _contextMenu;							/**< Pointer to the context menu */
	bool _running;										/**< Woopsi runs its main loop only if this is true*/

	/**
	 * Closes a child gadget.
	 * @param gadget The child gadget to close.
	 */
	virtual void closeChild(Gadget* gadget);
	
	/**
	 * Shelves a child gadget.
	 * @param gadget The child gadget to shelve.
	 */
	virtual void shelveChild(Gadget* gadget);

	/**
	 * Delete any gadgets in the deletion queue.
	 */
	void processDeleteQueue();

	/**
	 * Process stylus events and send throughout the hierarchy.
	 * @param gadget Sub-gadget to process, used for modal gadgets; omit
	 * this parameter to run the whole system.
	 */
	virtual void handleStylus(Gadget* gadget);

	/**
	 * Process keypad events and send throughout the hierarchy.
	 */
	virtual void handleKeys();

	/**
	 * Process lid events and send throughout the hierarchy.
	 */
	virtual void handleLid();

	/**
	 * Process VBLs and send to all registered gadgets.
	 */
	virtual void handleVBL();

	/**
	 * Deletes the system font.
	 */
	static void deleteSystemFont();

	/**
	 * Click the specified gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @param gadget Pointer to the gadget to click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y, Gadget* gadget);

	/**
	 * Shift-click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
 	 * @param gadget Pointer to the gadget to click.
	 * @return True if the click was successful.
	 */
	virtual bool shiftClick(s16 x, s16 y, Gadget* gadget);
};

/**
 * Use the following define to access the Woopsi singleton object.
 */
#define woopsiApplication (Woopsi::singleton)

#endif
