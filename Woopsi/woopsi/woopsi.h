#ifndef _WOOPSI_H_
#define _WOOPSI_H_

#include <nds.h>
#include <vector>
#include "gadget.h"

namespace WoopsiUI {

	class Screen;
	class ContextMenu;
	class WoopsiTimer;

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
		 * Initialise the application.  All initial GUI creation, hardware setup, etc, should
		 * be done in an override of this method.  The base method should be called as the
		 * first instruction in the overridden function.
		 * This method must call enableDrawing() and draw() at some point or nothing will
		 * be visible.
		 */
		virtual inline void startup() { };

		/**
		 * Run the gadget modally.  This will run the Woopsi application until stopModal()
		 * is called.
		 */
		void goModal();

		/**
		 * Shut down the application.  All non-gadget objects should be deleted in an override
		 * of this function, and all hardware should be shut down, etc.
		 * This base method will shut down the SDL system, so it must be called as the last
		 * function if overridden in an SDL application.  If you don't want to let Woopsi
		 * shut down SDL (if you have another chunk of code to run later, for example),
		 * you will need to shut down SDL yourself.
		 */
		virtual inline void shutdown() {
	#ifdef USING_SDL
			SDL_Quit();
	#endif
		};

		/**
		 * Run all code that needs to take place once a frame.
		 * This should be called every VBL in order for Woopsi to work.
		 * @param gadget Sub-gadget to run, used for modal gadgets; omit
		 * this parameter to run the whole system.
		 */
		virtual void processOneVBL(Gadget* gadget = NULL);
		
		/**
		 * Draw the entire Woopsi GUI to the display.
		 * Should be called once in the startup() method.
		 * @see startup()
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
		 * Add the supplied timer to the list of timers that receive VBL notifications.
		 * This is handled automatically by the WoopsiTimer constructor and should not be
		 * used in user code.
		 * @param timer A pointer to the timer to add to the VBL list.
		 */
		static void registerForVBL(WoopsiTimer* timer);

		/**
		 * Remove the supplied timer from the list of timers that receive VBL notifications.
		 * This is handled automatically by the WoopsiTimer destructor and should not be
		 * used in user code.
		 * @param timer A pointer to the timer to remove from the VBL list.
		 */
		static void unregisterFromVBL(WoopsiTimer* timer);

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
		 * Main entry point for a Woopsi application.
		 */
		virtual inline int main(int argc, char* argv[]) {
			startup();				// Run any setup code
			goModal();				// Run the main Woopsi loop
			shutdown();				// Run any shutdown code

			return 0;
		};

		/**
		 * Set the clicked gadget pointer.  Note that this should not be
		 * called by code other than within the Woopsi library itself.
		 * @param gadget The new clicked gadget.
		 */
		inline void setClickedGadget(Gadget* gadget) {

			// Do we have a clicked gadget already?
			if (_clickedGadget != NULL) {

				// Ensure that the existing clicked gadget is released *outside* its bounds
				_clickedGadget->release(_clickedGadget->getX() - 10, 0);
			}
			
			// Update the pointer
			_clickedGadget = gadget;
		};

		/**
		 * Get the clicked gadget pointer.  
		 * @return Pointer to the clicked gadget.
		 */
		inline Gadget* getClickedGadget() { return _clickedGadget; };

	protected:
		bool _lidClosed;									/**< Remembers the current state of the lid */
		
		static DynamicArray<WoopsiTimer*> _vblListeners;	/**< Array of timers that receive VBL events */
		static DynamicArray<Gadget*> _deleteQueue;			/**< Array of gadgets awaiting deletion */
		static FontBase* _systemFont;						/**< Pointer to the default font */
		static u32 _vblCount;								/**< Count of VBLs since Woopsi was first run */
		ContextMenu* _contextMenu;							/**< Pointer to the context menu */
		Gadget* _clickedGadget;								/**< Pointer to the gadget that is clicked */

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

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Woopsi(const Woopsi& woopsi) : Gadget(woopsi) { };
	};
}

/**
 * Use the following define to access the Woopsi singleton object.
 */
#define woopsiApplication (Woopsi::singleton)

#endif
