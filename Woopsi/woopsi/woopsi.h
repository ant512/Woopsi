#ifndef _WOOPSI_H_
#define _WOOPSI_H_

#include <nds.h>
#include <vector>
#include "sysfont.h"
#include "bitmap.h"
#include "fontbase.h"
#include "font.h"
#include "monofont.h"
#include "gadget.h"
#include "button.h"
#include "window.h"
#include "amigawindow.h"
#include "simplewindow.h"
#include "screen.h"
#include "amigascreen.h"
#include "simplescreen.h"
#include "textwriter.h"
#include "textbox.h"
#include "eventhandler.h"
#include "superbitmap.h"
#include "graphicsport.h"
#include "gradient.h"
#include "skin.h"
#include "skinnedscreen.h"
#include "skinnedwindow.h"
#include "debug.h"
#include "animation.h"
#include "bitmapbutton.h"
#include "animbutton.h"
#include "woopsifuncs.h"
#include "scrollingpanel.h"
#include "slidervertical.h"
#include "scrollbarvertical.h"

using namespace std;

/**
 * Class providing a top-level gadget and an interface to the Woopsi gadget hierarchy.
 */
class Woopsi : public Gadget {

public:
	Woopsi(FontBase* font = NULL);
	virtual ~Woopsi();
	
	/**
	 * Add a new screen to the Woopsi instance.
	 * @note This should be removed.
	 * @param title The title of the new screen.
	 */
	virtual Screen* newScreen(char* title);
	
	/**
	 * Run the Woopsi instace.
	 * @note This should be called every VBL in order for Woopsi to work.
	 */
	virtual void play();
	
	/**
	 * Draw the entire Woopsi GUI to the display.
	 * @note should be called before the main loop runs.
	 */
	virtual void draw();
	
	/**
	 * Draw a specific rectangle of the GUI.
	 * @note The supplied region will be drawn with no further clipping checks.
	 * @note For use only within Woopsi gadgets.
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
	 * @returns True if clicked.
	 */
	virtual bool click(s16 x, s16 y);

	/**
	 * Receive stylus releases and process them.
	 * @param x The x co-ordinate of the release.
	 * @param y The y co-ordinate of the release.
	 * @returns True if released.
	 */
	virtual bool release(s16 x, s16 y);

	/**
	 * Receive stylus drags and process them.
	 * @param x The current x co-ordinate of the stylus.
	 * @param y The current y co-ordinate of the stylus.
	 * @param vX The horizontal distance dragged.
	 * @param vY The vertical distance dragged.
	 * @returns True if dragged.
	 */
	virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	/**
	 * Receive and process VBLs.
	 * @returns True if VBL was processed.
	 */
	virtual bool vbl();

	/**
	 * Sets the active gadget pointer to the supplied gadget.
	 * @param gadget The gadget to point the internal pointer at; must be a child of this gadget.
	 */
	virtual void setActiveGadget(Gadget* gadget);

	/**
	 * Swaps the depth of the supplied gadget.
	 * @note This function presumes that all child gadgets are screens.
	 * @param gadget The gadget to be depth-swapped.
	 * @returns True if the depth swap occurred.
	 */
	virtual bool swapGadgetDepth(Gadget* gadget);

	/**
	 * Flips the supplied gadget from its current screen to the other screen.
	 * @note Thuis functions presumes that all child gadgets are screens.
	 * @param gadget The gadget (must be a screen) to flip.
	 * @returns True if the flip occurred.
	 */
	virtual bool flipScreens(Gadget* gadget);

	/**
	 * Redraws any dirty regions within the supplied region.
	 * @param rect The region to redraw
	 */
	virtual void eraseRect(Rect rect);

	/**
	 * Get a pointer to the system font.
	 * @returns A pointer to the system font.
	 */
	static FontBase* getSystemFont();

	/**
	 * Deletes the system font.
	 * Note that this should not be called.
	 */
	static void deleteSystemFont();

	/**
	 * Add the supplied gadget to the list of gadgets that receive VBL notifications.
	 * @note Gadgets not in this list will not be able to update automatically when the screen refreshes.
	 * @param A pointer to the gadget to add to the VBL list.
	 */
	static void registerForVBL(Gadget* gadget);

	/**
	 * Remove the supplied gadget from the list of gadgets that receive VBL notifications.
	 * @note Gadgets removed from the list will no longer be able to update automatically when the screen refreshes.
	 * @param gadget A pointer to the gadget to remove from the VBL list.
	 */
	static void unregisterFromVBL(Gadget* gadget);

	/**
	 * Add a gadget to the list of gadgets to be deleted.
	 * @note Must never be called by anything other than the framework itself.
	 */
	static void addToDeleteQueue(Gadget* gadget);

	/**
	 * Return the number of VBLs that have occurred since Woopsi began running.
	 * @note The count will eventually overflow the 32-bit int and reset to 0.  Developers must allow for this.
	 */
	static u32 getVBLCount();

protected:
	bool _lidClosed;
	
	static vector<Gadget*> _vblListeners;
	static vector<Gadget*> _deleteQueue;
	static FontBase* _systemFont;
	static u32 _vblCount;

	virtual void closeChild(Gadget* gadget);
	virtual void hideChild(Gadget* gadget);

	void processDeleteQueue();
};

#endif
