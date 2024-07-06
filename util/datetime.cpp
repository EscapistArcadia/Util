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

static const unsigned short kDayBeforeYear[4] = {0, 365, 730, 1096};

static const int kTimeTFactor = 1000;
static const int kNanosecondFactor = 1000000;
static const int kStartingYear = 1970;
static const int kRoundFactor = 365 * 4 + 1;

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

bool DateTime::IsLeapYear(unsigned short year) noexcept {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}
