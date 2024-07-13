/**
 * @brief None
 * @file datetime.cpp
 * @note None
 * @date July 06 2024, 09:04
 * @author Escapist
 */
#include "datetime.h"

#include <cassert>

static const short kDayInMonth[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},                        // normal years
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},                        // leap years
};

static const short kDayBeforeMonth[2][13] = {
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, /*365*/},        // normal years
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, /*366*/},        // leap years
};

static const short kDayInYear[4] = {365, 365, 366, 365};

static const short kDayBeforeYear[4] = {0, 365, 730, 1096};

static const short kStartingYear = 1970;
static const short kRoundFactor = 365 * 4 + 1;

enum DateTimeField {
    Second = 1,
    Minute = 60,
    Hour = 60 * Minute,
    Day = 24 * Hour,
};

Date::Date(short year, short month, short day) : year_(year), month_(month), day_(day) {
    assert(month >= 1 && month <= 12);
    assert(day >= 1 && day <= kDayInMonth[IsLeapYear(year)][month]);
}

short Date::GetDayOfWeek() const noexcept {
    return 0;
}

short Date::GetDayOfYear() const noexcept {
    return day_ + kDayBeforeMonth[IsLeapYear(year_)][month_];
}

Date &Date::SetDate(short year, short month, short day) noexcept {
    assert(month >= 1 && month <= 12);
    assert(day >= 1 && day <= kDayInMonth[IsLeapYear(year)][month]);
    year_ = year;
    month_ = month;
    day_ = day;
    return *this;
}

Date &Date::AddYear(short year) {
    year_ += year;
    if (day_ > kDayInMonth[IsLeapYear(year_)][month_]) {
        day_ = kDayInMonth[IsLeapYear(year_)][month_];
    }
    return *this;
}

Date &Date::AddMonth(short month) {
    month_ += month;
    if (month_ > 12) {
        AddYear(month_ / 12);
        month_ %= 12;
    }
    if (day_ > kDayInMonth[IsLeapYear(year_)][month_]) {
        day_ = kDayInMonth[IsLeapYear(year_)][month_];
    }
    return *this;
}

Date &Date::AddDay(short day) {
    day_ += day;
    while (day_ > kRoundFactor) {
        day_ -= kRoundFactor;
        year_ += 4;
    }
    bool leap = IsLeapYear(year_);
    for (int i = 0, leap = IsLeapYear(year_);
         (day_ >= (leap ? 366 : 365)) && i < 4;
         ++i, day_ -= leap ? 366 : 365, ++year_, leap = IsLeapYear(year_));
    while (day_ > kDayInMonth[leap][month_]) {
        day_ -= kDayInMonth[leap][month_];
        if ((++month_) > 12) {
            month_ = 1;
            leap = IsLeapYear(++year_);
        }
    }
    return *this;
}

Time &Time::SetHour(short hour) {
    assert(hour < 24);
    hour_ = hour;
    return *this;
}

Time &Time::SetMinute(short minute) {
    assert(minute < 60);
    minute_ = minute;
    return *this;
}

Time &Time::SetSecond(short second) {
    assert(second < 60);
    second_ = second;
    return *this;
}

DateTime::DateTime(short year, short month, short day, short hour, short minute, short second)
        : time_(year - kStartingYear) {                  /* stores the passed year */
    assert(second >= 0 && second < 60);         /* checks the validity */
    assert(minute >= 0 && minute < 60);
    assert(hour >= 0 && hour < 24);

    bool leap = Date::IsLeapYear(year);
    assert(year >= kStartingYear);              /* checks the validity */
    assert(month >= 1 && month <= 12);
    assert(day >= 1 && day <= kDayInMonth[leap][month]);

    //        each round                  days before the year        days before month              days in this month
    time_ = (((time_ / 4) * kRoundFactor + kDayBeforeYear[time_ % 4] + kDayBeforeMonth[leap][month] + day - 1) * Day);
    time_ += (hour * Hour + minute * Minute + second * Second);
}

Date DateTime::GetDate() const noexcept {
    Int64 day = (time_ / Day);

    short year = kStartingYear + ((day / kRoundFactor) << 2), month;
    day %= kRoundFactor;
    for (month = 0; month < 4 && day > kDayInYear[month];
         day -= kDayInYear[month], ++month, ++year);

    bool leap = Date::IsLeapYear(year);
    for (month = 1; month <= 12 && day > kDayInMonth[leap][month];
         day -= kDayInMonth[leap][month], ++month);

    return {year, month, static_cast<short>(day + 1)};
}

Time DateTime::GetTime() const noexcept {
    Int64 time = (time_ % Day);
    Time t;
    t.SetSecond(time % 60);
    time /= 60;
    t.SetMinute(time % 60);
    time /= 60;
    t.SetHour(time);
    return t;
}

short DateTime::GetYear() const noexcept {
    Int64 day = (time_ / Day);

    short year = kStartingYear + ((day / kRoundFactor) << 2), month;
    day %= kRoundFactor;
    for (month = 0; month < 4 && day > kDayInYear[month];
         day -= kDayInYear[month], ++month, ++year);
    return year;
}

short DateTime::GetMonth() const noexcept {
    return GetDate().GetMonth();
}

short DateTime::GetDay() const noexcept {
    return GetDate().GetDay();
}

short DateTime::GetHour() const noexcept {
    return (time_ / Hour) % 24;
}

short DateTime::GetMinute() const noexcept {
    return (time_ / Minute) % 60;
}

short DateTime::GetSecond() const noexcept {
    return time_ % 60;
}

DateTime &DateTime::SetDate(const Date &date) noexcept {
    Int64 day = date.GetYear() - kStartingYear;
    day = (kRoundFactor * (day / 4) + kDayBeforeYear[day % 4]
           + kDayBeforeMonth[Date::IsLeapYear(date.GetYear())][date.GetMonth()] + date.GetDay() - 1);
    time_ = day * Day + (time_ % Day);
    return *this;
}

DateTime &DateTime::SetTime(const Time &time) noexcept {
    Int64 second = time.GetHour() * Hour + time.GetMinute() * Minute + GetSecond() * Second;
    time_ = (time_ - time_ % Day) + second;
    return *this;
}

DateTime &DateTime::AddSeconds(Int64 seconds) noexcept {
    time_ += seconds;
    return *this;
}

DateTime &DateTime::AddMinutes(Int64 minutes) noexcept {
    return AddSeconds(minutes * Minute);
}

DateTime &DateTime::AddHours(Int64 hours) noexcept {
    return AddSeconds(hours * Hour);
}

DateTime &DateTime::AddDays(Int64 days) noexcept {
    return AddSeconds(days * Day);
}

DateTime &DateTime::AddMonths(Int64 months) noexcept {
    Int64 seconds = 0;
}

DateTime &DateTime::AddYears(Int64 years) {
    Int64 day = 0;
    short year = GetYear() - kStartingYear;
    if (years >= 0) {
        for (; years >= 4; day += kRoundFactor, years -= 4, year += 4);
        for (; years; day += kDayInYear[year & 3], --years, ++year);
    } else {
        for (; years > -4; day -= kRoundFactor, years += 4, year -= 4);
        for (; years; day -= kDayInYear[3 - (year & 3)], ++years, --year);
    }
    return AddDays(day);
}

short TimeSpan::GetDay() const noexcept {
    return diff_ / Day;
}

short TimeSpan::GetHour() const noexcept {
    return (diff_ % Day) / Hour;
}

Int64 TimeSpan::GetTotalHour() const noexcept {
    return diff_ / Hour;
}

short TimeSpan::GetMinute() const noexcept {
    return (diff_ % Hour) / Minute;
}

Int64 TimeSpan::GetTotalMinute() const noexcept {
    return diff_ / Minute;
}

short TimeSpan::GetSecond() const noexcept {
    return (diff_ % Minute);
}

Int64 TimeSpan::GetTotalSecond() const noexcept {
    return diff_;
}

TimeSpan &TimeSpan::AddSeconds(Int64 seconds) noexcept {
    diff_ += seconds;
    return *this;
}

TimeSpan &TimeSpan::AddMinutes(Int64 minutes) noexcept {
    return AddSeconds(minutes * Minute);
}

TimeSpan &TimeSpan::AddHours(Int64 hours) noexcept {
    return AddSeconds(hours * Hour);
}

TimeSpan &TimeSpan::AddDays(Int64 days) noexcept {
    return AddSeconds(days * Day);
}
