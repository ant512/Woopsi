#ifndef _TAB_GROUP_H_
#define _TAB_GROUP_H_

#include <nds.h>
#include "gadget.h"
#include "gadgetstyle.h"
#include "gadgeteventhandler.h"

namespace WoopsiUI {

	class Tab;

	/**
	 * Container class that holds tab gadgets and tracks their status.  The
	 * group provides an easy way to determine which tab is selected.
	 */
	class TabGroup : public Gadget, public GadgetEventHandler  {

	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the group.
		 * @param y The y co-ordinate of the group.
		 * @param width The width of the group.
		 * @param height The height of the group.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		TabGroup(s16 x, s16 y, s16 width, s16 height, GadgetStyle* style = NULL);

		/**
		 * Adds a new tab to the group.
		 * This should be used in preference to the usual addGadget() method,
		 * as this method automatically adjusts the other tabs to fit.
		 */
		Tab* newTab(const WoopsiString& text);

		/**
		 * Gets a pointer to the selected gadget.
		 * @return Pointer to the selected gadget.
		 */
		virtual const Tab* getSelectedGadget() const;

		/**
		 * Gets the index of the selected gadget.
		 * @return The index of the selected gadget.
		 */
		virtual const s32 getSelectedIndex() const;

		/**
		 * Sets the selected tab to the supplied gadget.
		 * @param gadget The tab to select.
		 */
		virtual void setSelectedGadget(Tab* gadget);

		/**
		 * Selects the gadget at the specified index.
		 * @param index The index of the gadget to select.
		 */
		virtual void setSelectedIndex(s32 index);

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the
		 * gadget's parent.  Value is based on the length of the largest string
		 * in the set of options.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

		/**
		 * Handle a stylus click event.
		 * @param source The gadget that was clicked.
		 * @param point The co-ordinates of the click.
		 */
		virtual void handleClickEvent(Gadget& source, const WoopsiPoint& point);

		/**
		 * Handle a gadget double-click event.
		 * @param source The gadget that was clicked.
		 * @param point The co-ordinates of the click.
		 */
		virtual void handleDoubleClickEvent(Gadget& source, const WoopsiPoint& point);

		/**
		 * Handle a stylus release event that occurred within the bounds of
		 * the source gadget.
		 * @param source The gadget that was released.
		 * @param point The co-ordinates of the release.
		 */
		virtual void handleReleaseEvent(Gadget& source, const WoopsiPoint& point);

		/**
		 * Handle a stylus release event that occurred outside the bounds of
		 * the source gadget.
		 * @param source The gadget that was released.
		 * @param point The co-ordinates of the release.
		 */
		virtual void handleReleaseOutsideEvent(Gadget& source, const WoopsiPoint& point);

	protected:
		Tab* _selectedGadget;				/**< Pointer to the currently
												 selected tab */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Destructor.
		 */
		virtual ~TabGroup() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline TabGroup(const TabGroup& tabGroup) : Gadget(tabGroup) { };
	};
}

#endif
