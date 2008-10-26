#ifndef _DATE_H_
#define _DATE_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Class representing a date.
	 */
	class Date {
	public:

		/**
		 * Constructor.
		 * @param day The day of this date.
		 * @param month The month of this date.
		 * @param year The year of this date.
		 */
		inline Date(u8 day, u8 month, u16 year) {
			setDate(day, month, year);
		};

		/**
		 * Destructor.
		 */
		inline ~Date() { };

		/**
		 * Get the name of the current day.
		 * @return The name of the current day.
		 */
		inline const char* getDayName() const { return _dayNames[_weekDay]; };

		/**
		 * Get the name of the current month.
		 * @return The name of the current month.
		 */
		inline const char* getMonthName() const { return _monthNames[_month - 1]; };

		/**
		 * Get the integer value of the current day of the week.  Value is 0-based
		 * where 0 represents Sunday and 6 represents Saturday.
		 * @return The current day of the week.
		 */
		inline const u8 getWeekDay() const { return _weekDay; };

		/**
		 * Returns true if the year is a leap year.
		 * @return True if the year is a leap year.
		 */
		inline const bool isLeapYear() const {
			return ((_year % 4 == 0) && ((_year % 100 != 0) || (_year % 400 == 0)));
		};

		/**
		 * Get the number of days in the current year.
		 * @return The number of days in the current year.
		 */
		inline const u16 getYearDays() {
			if (isLeapYear()) return 366;
			return 365;
		}

		/**
		 * Get the number of days in the current month.
		 * @return The number of days in the current month.
		 */
		inline const u8 getMonthDays() {
			switch (_month) {
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					// Jan, Mar, May, Jul, Aug, Oct, Dec = 31 days
					return 31;
				case 4:
				case 6:
				case 9:
				case 11:
					// Apr, Jun, Sept, Nov = 30 days
					return 30;
				case 2:
					// Feb = 28 or 29 depending on year
					if (isLeapYear()) return 29;
					return 28;
			}

			// Month not an expected value
			return 0;
		};

		/**
		 * Add a number of years to this date.
		 * @param years Number of years to add.
		 */
		inline void addYears(s16 years) {
			setYear(years + _year);
			calculateWeekDay();
		};

		/**
		 * Add a number of months to this date.  Automatically rolls over years if necessary.
		 * @param months Number of months to add.
		 */
		inline void addMonths(s16 months) {

			// Handle negative months
			if (months < 0) {

				// Roll back years
				addYears(months / 12);
				months %= 12;

				// Roll back another year if the abs(months) to roll back by
				// exceed the current month
				if (0 - months >= _month) {
					addYears(-1);

					// Compensate for the fact we've already rolled back a year
					months = months + 12;
				}
			}

			// Add months - even if months is negative, we can guarantee that months
			// plus _month is >= 1.
			setMonth(months + _month);
			calculateWeekDay();
		};

		/**
		 * Add a number of days to this date.
		 * @param days Number of days to add.
		 */
		inline void addDays(s16 days) {

			// Handle negative days
			if (days < 0) {

				// Roll back months
				u8 monthDays = getMonthDays();

				while (days < -_day) {
					days += monthDays;
					addMonths(-1);
					monthDays = getMonthDays();
				}
			}

			// Add days - even if days if negative, we can guarantee that days
			// plus _day >= 0.
			setDay(days + _day);
			calculateWeekDay();
		};

		/**
		 * Get the day.
		 * @return The day.
		 */
		inline const u8 getDay() const { return _day; };

		/**
		 * Get the month.
		 * @return The month.
		 */
		inline const u8 getMonth() const { return _month; };

		/**
		 * Get the year.
		 * @return The year.
		 */
		inline const u16 getYear() const { return _year; };

		/**
		 * Set the year.
		 * @param day The day.
		 * @param month The month.
		 * @param year The year.
		 */
		inline void setDate(u8 day, u8 month, u16 year) {
			setYear(year);
			setMonth(month);
			setDay(day);

			calculateWeekDay();
		};

	private:
		u8 _day;
		u8 _month;
		u16 _year;
		u8 _weekDay;
		static const char* _dayNames[7];
		static const char* _monthNames[12];

		/**
		 * Returns 1 if the date falls before the 29th of Feb and the year is a leap year.
		 * Necessary for correct calculation of year offset.
		 * @return 1 if date before 29th Feb in leap year; 0 otherwise.
		 */
		inline const int getLeapCompensationValue() const {
			if ((_month < 3) && (_day < 29) && isLeapYear()) {
				return 1;
			} else {
				return 0;
			}
		};

		/**
		 * Calculates the week day represented by the current day.
		 */
		inline void calculateWeekDay() {

			u8 centuryOffset = 0;
			u8 yearOffset = 0;
			u8 monthOffset = 0;
			u8 dayOffset = 0;

			centuryOffset = ((39 - (_year / 100)) % 4) << 1;
			yearOffset = ((_year % 100) + (((_year % 100) / 4) - getLeapCompensationValue())) % 7;
		
			// Calculate month offset
			switch (_month) {
				case 1:
				case 10:
					// Jan, Oct
					monthOffset = 0;
					break;
				case 2:
				case 3:
				case 11:
					// Feb, Mar, Nov
					monthOffset = 3;
					break;
				case 4:
				case 7:
					// Apr, Jul
					monthOffset = 6;
					break;
				case 5:
					// May
					monthOffset = 1;
					break;
				case 6:
					// Jun
					monthOffset = 4;
					break;
				case 8:
					// Aug
					monthOffset = 2;
					break;
				case 9:
				case 12:
					// Sept, Dec
					monthOffset = 5;
					break;
			}

			dayOffset = _day % 7;

			_weekDay = (centuryOffset + yearOffset + monthOffset + dayOffset) % 7;
		};

		/**
		 * Set the year.
		 * @param year The new year.
		 */
		void setYear(u16 year) {

			// Enforce minimum year 1900 
			if (year >= 1900) {
				_year = year;
			} else {
				_year = 1900;
			}
		};

		/**
		 * Set the month.  Automatically wraps around years if the month value is not
		 * valid.
		 * @param month The new month.
		 */
		void setMonth(u8 month) {

			// Catch 0 months and force to 1
			if (month == 0) {
				_month = 1;
				return;
			}

			// Catch months above expected range and wrap around
			if (month > 12) {
				_year += month / 12;
				_month = month % 12;
				return;
			}

			// Normal behaviour
			_month = month;
		};

		/**
		 * Set the day.  Automatically wraps around days if the day value is not valid.
		 * @param day The new day.
		 */
		void setDay(u8 day) {

			// Catch 0 days
			if (day == 0) {
				_day = 1;
				return;
			}

			// Get the number of days in this month
			u8 maxDays = getMonthDays();

			// If the requested day exceeds the number of days in the current month,
			// reduce the day by the number of days in the month and roll over the month.
			// Year rollover is handled by the month rollover.
			while (day > maxDays) {
				day -= maxDays;
				setMonth(_month + 1);
				maxDays = getMonthDays();
			}

			_day = day;
		};
	};
}

#endif
