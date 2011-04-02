#ifndef _WOOPSI_H_
#define _WOOPSI_H_

#include <nds.h>
#include "gadget.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	class Screen;
	class ContextMenu;
	class WoopsiTimer;
	class WoopsiKeyboardScreen;
	class KeyboardEventHandler;
	class DamagedRectManager;

	/**
	 * Class providing a top-level gadget and an interface to the Woopsi gadget
	 * hierarchy.
	 */
	class Woopsi : public Gadget {

	public:
		/**
		 * The singleton provides an instance of Woopsi that should be
		 * instantiated and used as the only Woopsi instance.  The Debug class
		 * in particular interacts with this singleton.
		 */
		static Woopsi *singleton;
		
		/**
		 * Constructor.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		Woopsi(GadgetStyle* style = NULL);
		
		/**
		 * Destructor.
		 */
		virtual ~Woopsi();

		/**
		 * Run all code that needs to take place once a frame.  This should be
		 * called every VBL in order for Woopsi to work.
		 * @param gadget Sub-gadget to run, used for modal gadgets; omit
		 * this parameter to run the whole system.
		 */
		virtual void processOneVBL(Gadget* gadget = NULL);

		/**
		 * Run the gadget modally.  This will run the Woopsi application until
		 * stopModal() is called.
		 */
		void goModal();

		/**
		 * Flips the supplied gadget from its current screen to the other
		 * screen.  This function presumes that all child gadgets are screens.
		 * @param gadget The gadget (must be a screen) to flip.
		 * @return True if the flip occurred.
		 */
		virtual bool flipScreens(Gadget* gadget);

		/**
		 * Add the supplied timer to the list of timers that receive VBL
		 * notifications.  This is handled automatically by the WoopsiTimer
		 * constructor and should not be used in user code.
		 * @param timer A pointer to the timer to add to the VBL list.
		 */
		void registerForVBL(WoopsiTimer* timer);

		/**
		 * Remove the supplied timer from the list of timers that receive VBL
		 * notifications.  This is handled automatically by the WoopsiTimer
		 * destructor and should not be used in user code.
		 * @param timer A pointer to the timer to remove from the VBL list.
		 */
		void unregisterFromVBL(WoopsiTimer* timer);

		/**
		 * Add a gadget to the list of gadgets to be deleted.  Must never be
		 * called by anything other than the framework itself.
		 * @param gadget The gadget to add to the delete queue.
		 */
		void addToDeleteQueue(Gadget* gadget);

		/**
		 * Return the number of VBLs that have occurred since Woopsi began
		 * running.  The count will eventually overflow the 32-bit int and reset
		 * to 0.  Developers must allow for this.
		 * @return The VBL count.
		 */
		inline u32 getVBLCount() { return _vblCount; };

		/**
		 * Get a pointer to the context menu.
		 * @return Pointer to the context menu.
		 */
		inline ContextMenu* getContextMenu() { return _contextMenu; };

		/**
		 * Shelve the context menu.  Hides it without destroying the object and
		 * without it remaining an active, but hidden, gadget.
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
		void setClickedGadget(Gadget* gadget);

		/**
		 * Get the clicked gadget pointer.  
		 * @return Pointer to the clicked gadget.
		 */
		inline Gadget* getClickedGadget() { return _clickedGadget; };

		/**
		 * Creates a keyboard screen.  Opens it on the bottom display and flips
		 * the screen currently on the bottom display up to the top screen.
		 * Automatically sets opener to be an event handler of the keyboard.
		 * @param opener The keyboard event handler that called the method.
		 */
		void showKeyboard(KeyboardEventHandler* opener);
		
		/**
		 * Closes the keyboard screen.  Flips the screen that was flipped to the
		 * top display by the call to showKeyboard() back down to the bottom
		 * display.
		 */
		void hideKeyboard();

		/**
		 * Get a pointer to the damaged rect manager.
		 * @return A pointer to the damaged rect manager.
		 */
		DamagedRectManager* getDamagedRectManager() { return _damagedRectManager; };

	protected:
		bool _lidClosed;									/**< Remembers the current state of the lid. */
		
		WoopsiArray<WoopsiTimer*> _vblListeners;			/**< Array of timers that receive VBL events. */
		WoopsiArray<Gadget*> _deleteQueue;					/**< Array of gadgets awaiting deletion. */
		u32 _vblCount;										/**< Count of VBLs since Woopsi was first run. */
		ContextMenu* _contextMenu;							/**< Pointer to the context menu. */
		Gadget* _clickedGadget;								/**< Pointer to the gadget that is clicked. */
		WoopsiKeyboardScreen* _keyboardScreen;				/**< Screen containing the popup keyboard. */
		DamagedRectManager* _damagedRectManager;			/**< Maintains damaged rect list and controls redraws. */

		/**
		 * Initialise the application.  All initial GUI creation, hardware
		 * setup, etc, should be done in an override of this method.
		 */
		virtual void startup() = 0;

		/**
		 * Shut down the application.  All non-gadget objects should be deleted
		 * in an override of this function, and all hardware should be shut
		 * down, etc.  This base method will shut down the SDL system, so it
		 * must be called as the last function if overridden in an SDL
		 * application.  If you don't want to let Woopsi shut down SDL (if you
		 * have another chunk of code to run later, for example), you will need
		 * to shut down SDL yourself.
		 */
		virtual inline void shutdown() {
	#ifdef USING_SDL
			SDL_Quit();
	#endif
		};

		/**
		 * Pass clicks to the gadget hierarchy.  Closes the context menu if the
		 * clicked gadget is not the context menu.  If a single gadget is
		 * supplied, only that gadget is sent the click.  That gadget should be
		 * running modally.
		 * @param x Stylus x co-ordinate.
		 * @param y Stylus y co-ordinate.
		 * @param gadget Pointer to a modally-running gadget or NULL.
		 */
		void handleClick(s16 x, s16 y, Gadget* gadget);

		/**
		 * Pass shift-clicks to the gadget hierarchy.  If a single gadget is
		 * supplied, only that gadget is sent the click.  That gadget should be
		 * running modally.
		 * @param x Stylus x co-ordinate.
		 * @param y Stylus y co-ordinate.
		 * @param gadget Pointer to a modally-running gadget or NULL.
		 */
		void handleShiftClick(s16 x, s16 y, Gadget* gadget);

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
		 * Processes a single keypad event and sends it throughout the
		 * hierarchy.  Called by handleKeys().
		 * @param newPress Indicates whether or not this is a new press.
		 * @param released Indicates whether or not the keypad has been
		 * released.
		 * @param heldTime The number of frames that the key has been held down.
		 * @param keyCode The code of the button that has been pressed.
		 */
		void handleKey(bool newPress, bool released, s32& heldTime, KeyCode keyCode);

		/**
		 * Process lid events and send throughout the hierarchy.
		 */
		virtual void handleLid();

		/**
		 * Process VBLs and send to all registered gadgets.
		 */
		virtual void handleVBL();

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
