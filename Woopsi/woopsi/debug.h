#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <nds.h>
#include "eventhandler.h"

#define DEBUG_ACTIVE true

class AmigaScreen;
class AmigaWindow;
class MultiLineTextBox;
class Gadget;
class Woopsi;
class Font;
class SliderVertical;

/**
 * Debugging console.  The class automatically creates its own
 * screen and full GUI when one of the output routines is called.
 *
 * The DEBUG_ACTIVE define must be set to "true" for the debugger
 * to work.  If this is defined as "false", calls to the debugger
 * will silently be ignored.
 */
class Debug : public EventHandler {

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

	/**
	 * Handles all child gadget events.
	 * @param e The event arguments to be processed.
	 */
	bool handleEvent(const EventArgs& e);

private:
	static Debug* _debug;
	static Woopsi* _woopsi;
	AmigaScreen* _screen;
	AmigaWindow* _window;
	MultiLineTextBox* _textBox;
	SliderVertical* _slider;
	Font* _font;

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
	void output(char* msg);
};

#endif
