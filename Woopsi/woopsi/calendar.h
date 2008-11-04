#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#include <nds.h>
#include "gadget.h"
#include "eventhandler.h"

#define CALENDAR_ROWS 6
#define CALENDAR_COLS 7

namespace WoopsiUI {

	class Button;
	class Label;
	class Date;

	/**
	 * Class providing a calendar/date picker.  Raises an EVENT_ACTION event
	 * when a new date is clicked.
	 */
	class Calendar : public Gadget, public EventHandler {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the calendar.
		 * @param y The y co-ordinate of the calendar.
		 * @param width The width of the calendar.
		 * @param height The height of the calendar.
		 * @param day The default day to display.
		 * @param month The default month to display.
		 * @param year The default year to display.
		 * @param flags Standard flags.
		 * @param font Optional font to use for text output.
		 */
		Calendar(s16 x, s16 y, u16 width, u16 height, u8 day, u8 month, u16 year, u32 flags, FontBase* font = NULL);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event data to process.
		 */
		virtual bool handleEvent(const EventArgs& e);

		/**
		 * Draw the region of the calendar within the clipping rect. Should not be called
		 * directly.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);
		
		/**
		 * Draw all visible regions of the calendar.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Set the date displayed in the calendar.
		 * @param day The new day.
		 * @param month The new month.
		 * @param year The new year.
		 */
		virtual void setDate(u8 day, u8 month, u16 year);

		/**
		 * Get the day displayed in the calendar.
		 * @return The day.
		 */
		const u8 getDay() const;

		/**
		 * Get the month displayed in the calendar.
		 * @return The month.
		 */
		const u8 getMonth() const;

		/**
		 * Get the year displayed in the calendar.
		 * @return The year.
		 */
		const u16 getYear() const;

		/**
		 * Resize the calendar to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		bool resize(u16 width, u16 height);

	protected:
		Date* _date;
		Date* _visibleDate;
		Button* _selectedDayButton;
		Button* _leftArrow;
		Button* _rightArrow;
		Label* _monthLabel;

		/**
		 * Destructor.
		 */
		virtual ~Calendar();

		/**
		 * Creates all gadgets but leaves them textless.
		 */
		virtual void buildGUI();

		/**
		 * Populates the previously-created gadget layout with correct text
		 * based on current date objects.
		 */
		virtual void populateGUI();

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Calendar(const Calendar& calendar) : Gadget(calendar) { };
	};
}

#endif
