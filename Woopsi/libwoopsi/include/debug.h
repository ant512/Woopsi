#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <nds.h>

#define DEBUG_ACTIVE true

namespace WoopsiUI {

	class AmigaScreen;
	class AmigaWindow;
	class Gadget;
	class Woopsi;
	class MonoFont;
	class ScrollingTextBox;

	/**
	 * Debugging console.  The class automatically creates its own
	 * screen and full GUI when one of the output routines is called.
	 *
	 * The DEBUG_ACTIVE define must be set to "true" for the debugger
	 * to work.  If this is defined as "false", calls to the debugger
	 * will silently be ignored.
	 */
	class Debug {

	public:
		/**
		 * Freeze program until the B button is pressed.
		 */
		static void busyWait();

		/**
		 * Output formatted values to the debug console.  Uses
		 * standard printf() syntax.
		 * @param format Format of the string to print.
		 * @param ... The values to output.
		 */
		static void printf(const char *format, ...);

	private:
		static Debug* _debug;					/**< Pointer to the debug singleton */
		AmigaScreen* _screen;					/**< Pointer to the debug screen */
		AmigaWindow* _window;					/**< Pointer to the debug window */
		ScrollingTextBox* _textBox;				/**< Pointer to the debug output textbox */
		MonoFont* _font;						/**< Pointer to the debug font */

		/** 
		 * Constructor is private to force a singleton pattern.
		 */
		Debug();

		/**
		 * Private destructor to force a singleton pattern.
		 */
		~Debug();

		/**
		 * Create the singleton instance of the debugger if it does
		 * not exist.
		 */
		static void createDebug();

		/**
		 * Create the debugging console GUI if it does not exist.
		 */
		void createGUI();

		/**
		 * Print a string to the debug console.
		 * @param msg The string to print.
		 */
		void output(const char* msg);
	};
}

#endif
