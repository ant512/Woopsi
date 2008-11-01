#include "date.h"

using namespace WoopsiUI;

const char* Date::_dayNames[7] = {
							"Sunday", "Monday", "Tuesday", "Wednesday",
							"Thursday", "Friday", "Saturday"};
const char* Date::_monthNames[12] = {
							"January", "February", "March", "April",
							"May", "June", "July", "August", "September",
							"October", "November", "December"};

void Date::addDays(s16 days) {

	// Handle negative days
	if (days < 0) {
		while (-days >= (s16)_day) {
			
			// Roll back months
			addMonths(-1);
			u8 monthDays = getMonthDays();
			days += monthDays;
		}
	}

	// Add days - even if days if negative, we can guarantee that days
	// plus _day >= 0.
	setDay(days + _day);
	calculateWeekDay();
}

void Date::addMonths(s16 months) {

	// Handle negative months
	if (months < 0) {

		// Roll back years
		addYears(months / 12);
		months %= 12;

		// Roll back another year if the abs(months) to roll back by
		// exceed the current month
		if (-months >= _month) {
			addYears(-1);

			// Compensate for the fact we've already rolled back a year
			months = months + 12;
		}
	}

	// Add months - even if months is negative, we can guarantee that months
	// plus _month is >= 1.
	setMonth(months + _month);
	calculateWeekDay();
}

void Date::addYears(s16 years) {
	setYear(years + _year);
	calculateWeekDay();
}

const u8 Date::getMonthDays() const {
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
}

void Date::setDate(u8 day, u8 month, u16 year) {
	setYear(year);
	setMonth(month);
	setDay(day);

	calculateWeekDay();
}

void Date::setDay(u8 day) {

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
}

void Date::setMonth(u8 month) {

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
}

void Date::setYear(u16 year) {

	// Enforce minimum year 1900 
	if (year >= 1900) {
		_year = year;
	} else {
		_year = 1900;
	}
}

void Date::calculateWeekDay() {

	u8 aa = (14 - _month) / 12;
	u16 yy = _year - aa;
	u16 mm = (_month + (12 * aa)) - 2;

	u8 dayOfWeek = (_day + yy + (yy / 4) - (yy / 100) + (yy / 400) + ((31 * mm) / 12)) % 7;

	// Get day of week in ISO format
	_weekDay = ((dayOfWeek + 6) % 7) + 1;
}

const int Date::getLeapCompensationValue() const {
	if ((_month < 3) && (_day < 29) && isLeapYear()) {
		return 1;
	} else {
		return 0;
	}
}

bool Date::operator==(const Date& date) const {
	return ((date.getDay() == _day) && (date.getMonth() == _month) && (date.getYear() == _year));
}

bool Date::operator!=(const Date& date) const {
	return ((date.getDay() != _day) || (date.getMonth() != _month) || (date.getYear() != _year));
}
