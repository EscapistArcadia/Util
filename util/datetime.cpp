/**
 * @brief None
 * @file datetime.cpp
 * @note None
 * @date July 06 2024, 09:04
 * @author Escapist
 */
#include "datetime.h"

#include <cassert>

static const unsigned short kDayInMonth[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},                        // normal years
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},                        // leap years
};

static const unsigned short kDayBeforeMonth[2][13] = {
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, /*365*/},        // normal years
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, /*366*/},        // leap years
};

static const unsigned short kDayInYear[4] = {365, 365, 366, 365};

static const unsigned short kDayBeforeYear[4] = {0, 365, 730, 1096};

static const int kTimeTFactor = 1000;
static const int kNanosecondFactor = 1000000;
static const int kStartingYear = 1970;
static const int kRoundFactor = 365 * 4 + 1;

Date::Date(unsigned short year, unsigned short month, unsigned short day)
        : year_(year), month_(month), day_(day) {
    assert(month >= 1 && month <= 12);
    assert(day >= 1 && day <= kDayInMonth[DateTime::IsLeapYear(year)][month]);
}

Date &Date::SetYear(unsigned short year) {
    year_ = year;
    return *this;
}

Date &Date::SetMonth(unsigned short month) {
    assert(month >= 1 && month <= 12);
    month_ = month;
    return *this;
}

Date &Date::SetDay(unsigned short day) {
    assert(day >= 1 && day <= kDayInMonth[Date::IsLeapYear(year_)][month_]);
    day_ = day;
    return *this;
}

bool Date::IsLeapYear(unsigned short year) noexcept {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

Date &Date::AddYear(unsigned short year) {
    year_ += year;
    return *this;
}

Date &Date::AddMonth(unsigned short month) {
    
}

Date &Date::AddDay(unsigned short day) {

}

Date &Date::SubtractYear(unsigned short year) {

}

Date &Date::SubtractMonth(unsigned short month) {

}

Date &Date::SubtractDay(unsigned short day) {

}

Time &Time::SetHour(unsigned short hour) {
    assert(hour < 24);
    hour_ = hour;
    return *this;
}

Time &Time::SetMinute(unsigned short minute) {
    assert(minute < 60);
    minute_ = minute;
    return *this;
}

Time &Time::SetSecond(unsigned short second) {
    assert(second < 60);
    second_ = second;
    return *this;
}

Time &Time::SetMillisecond(unsigned short millisecond) {
    assert(millisecond < 1000);
    millisecond_ = millisecond;
    return *this;
}

DateTime::DateTime(time_t time) noexcept: time_(time * kTimeTFactor) {}

DateTime::DateTime(const timespec &time) noexcept
        : time_(time.tv_sec * kTimeTFactor + time.tv_nsec / kNanosecondFactor) {}

DateTime::DateTime(const tm &time) : DateTime(time.tm_year, time.tm_mon + 1, time.tm_mday,
                                              time.tm_hour, time.tm_min, time.tm_sec, 0) {}

DateTime::DateTime(unsigned short year, unsigned short month, unsigned short day,
                   unsigned short hour, unsigned short minute, unsigned short second, unsigned short millisecond)
        : time_(year - kStartingYear) {
    assert(second < 60 && minute < 60 && hour < 23);
    assert(year >= kStartingYear && month >= 1 && month <= 12 && day < kDayInMonth[IsLeapYear(year)][month]);

    time_ = ((time_ >> 2) * kRoundFactor + kDayBeforeYear[time_ & 0x03]
             + kDayBeforeMonth[IsLeapYear(year)][month] + day - 1);
    time_ = (time_ * 24) + hour;
    time_ = (time_ * 60) + minute;
    time_ = (time_ * 60) + second;
    time_ = (time_ * 1000) + millisecond;
}

DateTime::DateTime(const Calendar &time)
        : DateTime(time.year, time.month, time.day, time.hour, time.minute, time.second, time.millisecond) {}

Date DateTime::GetDate() const noexcept {
    long long day = (time_ / (24 * 60 * 60 * 1000));
    unsigned short year = 1970 + 4 * (day / kRoundFactor), month;
    day %= kRoundFactor;
    for (int i = 0; i < 4 && day >= kDayInYear[i]; day -= kDayInYear[i], ++i, ++year);
    bool leap = Date::IsLeapYear(year);
    for (month = 1; month <= 12 && day > kDayInMonth[leap][month]; day -= kDayInMonth[leap][month], ++month);
    return {year, month, static_cast<unsigned short>(day + 1)};
}

unsigned short DateTime::GetYear() const noexcept {
    return GetDate().GetYear();
}

unsigned short DateTime::GetMonth() const noexcept {
    return GetDate().GetMonth();
}

unsigned short DateTime::GetDay() const noexcept {
    return GetDate().GetDay();
}

Time DateTime::GetTime() const noexcept {
    long long time = time_;
    Time t{};
    t.millisecond_ = time % 1000;
    time /= 1000;
    t.second_ = time % 60;
    time /= 60;
    t.minute_ = time % 60;
    time /= 60;
    t.hour_ = time % 24;
    return t;
}

unsigned short DateTime::GetHour() const noexcept {
    return (time_ / (1000 * 60 * 60)) % 24;
}

unsigned short DateTime::GetMinute() const noexcept {
    return (time_ / (1000 * 60)) % 60;
}

unsigned short DateTime::GetSecond() const noexcept {
    return (time_ / 1000) % 60;
}

unsigned short DateTime::GetMillisecond() const noexcept {
    return time_ % 1000;
}



bool DateTime::IsLeapYear(unsigned short year) noexcept {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

DateTime DateTime::Current(bool includeMillisecond) noexcept {
    if (includeMillisecond) {
        timespec ts{};
        if (timespec_get(&ts, TIME_UTC)) {
            return DateTime(ts);
        }
    }
    return DateTime(time(nullptr));
}
