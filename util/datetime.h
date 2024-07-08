/**
 * @brief None
 * @file datetime.h
 * @note None
 * @date July 06 2024, 09:04
 * @author Escapist
 */

#ifndef UTIL_DATETIME_H
#define UTIL_DATETIME_H

#include "platforminfo.h"
#include <ctime>

enum Field : long {
    Millisecond = 1,
    Second = 1000 * Millisecond,
    Minute = 60 * Second,
    Hour = 60 * Minute,
    Day = 24 * Hour,
    Week = 7 * Day,
    Month = 30 * Day,
    Year = 365 * Day,
    Round = (365 * 4 + 1) * Day
};

class Date {
public:
    Date() noexcept = default;

    Date(unsigned short year, unsigned short month, unsigned short day);

    Date(const Date &other) noexcept = default;

    ~Date() noexcept = default;

    unsigned short GetYear() const noexcept {
        return year_;
    }

    unsigned short GetMonth() const noexcept {
        return month_;
    }

    unsigned short GetDay() const noexcept {
        return day_;
    }

    Date &SetYear(unsigned short year);

    Date &SetMonth(unsigned short month);

    Date &SetDay(unsigned short day);

    Date &AddYear(unsigned short year);

    Date &AddMonth(unsigned short month);

    Date &AddDay(unsigned short day);

    Date &SubtractYear(unsigned short year);

    Date &SubtractMonth(unsigned short month);

    Date &SubtractDay(unsigned short day);

    static bool IsLeapYear(unsigned short year) noexcept;

private:
    friend class DateTime;

    unsigned short year_;
    unsigned short month_;
    unsigned short day_;
};

class Time {
public:
    Time() noexcept = default;

    Time(unsigned short hour, unsigned short minute, unsigned short second);

    Time(const Time &other) noexcept = default;

    ~Time() noexcept = default;

    unsigned short GetHour() const noexcept {
        return hour_;
    };

    unsigned short GetMinute() const noexcept {
        return minute_;
    }

    unsigned short GetSecond() const noexcept {
        return second_;
    }

    unsigned short GetMillisecond() const noexcept {
        return millisecond_;
    }

    Time &SetHour(unsigned short hour);

    Time &SetMinute(unsigned short minute);

    Time &SetSecond(unsigned short second);

    Time &SetMillisecond(unsigned short millisecond);

private:
    friend class DateTime;

    unsigned short hour_;
    unsigned short minute_;
    unsigned short second_;
    unsigned short millisecond_;
};

struct Calendar {
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned short millisecond;
};

enum class TimeZone {
    UTC_N12 = -(12 << 2), UTC_N11 = -(11 << 2), UTC_N10 = -(10 << 2), UTC_N09 = -(9 << 2), UTC_N08 = -(8 << 2),
    UTC_N07 = -(7 << 2), UTC_N06 = -(6 << 2), UTC_N05 = -(5 << 2), UTC_N04 = -(4 << 2), UTC_N03 = -(3 << 2),
    UTC_N02 = -(2 << 2), UTC_N01 = -(1 << 2),
    UTC = 0,
    UTC_01 = 1 << 2, UTC_02 = 2 << 2, UTC_03 = 3 << 2, UTC_04 = 4 << 2, UTC_05 = 5 << 2, UTC_06 = 6 << 2,
    UTC_07 = 7 << 2, UTC_08 = 8 << 2, UTC_09 = 9 << 2, UTC_10 = 10 << 2, UTC_11 = 11 << 2, UTC_12 = 12 << 2,
    UTC_13 = 13 << 2, UTC_14 = 14 << 2
};

class TimeZoneInfo {
public:
    TimeZoneInfo() = delete;

    constexpr explicit TimeZoneInfo(TimeZone tz) noexcept: offset_((long long) tz * 3600 >> 2) {}

    constexpr explicit TimeZoneInfo(long long offset) noexcept: offset_(offset) {}

    constexpr TimeZoneInfo(const TimeZoneInfo &other) noexcept = default;

    constexpr TimeZoneInfo(TimeZoneInfo &&other) noexcept = default;

    ~TimeZoneInfo() noexcept = default;

    const char_t *GetName() const noexcept;

    long long GetOffset() const noexcept;

    TimeZoneInfo &Set(TimeZone &tz) noexcept;

    static TimeZoneInfo Local() noexcept;

private:
    // static const TimeZoneInfo kLocalTimeZone;
    long long offset_;
};

/**
 * \c DateTime records milliseconds starting from UTC 00:00:00, Jan. 1st, 1970.
 * \c DateTime provides simple conversion between regular calendar (e.g.,
 * month/day/year hour:minute:second.millisecond) and timestamps.
 */
class DateTime {
public:
    DateTime() noexcept: time_(0) {}

    explicit DateTime(time_t time) noexcept;

    explicit DateTime(const timespec &time) noexcept;

    explicit DateTime(const tm &time);

    DateTime(unsigned short year, unsigned short month, unsigned short day,
             unsigned short hour, unsigned short minute, unsigned short second, unsigned short millisecond);

    explicit DateTime(const Calendar &time);

    DateTime(const DateTime &other) noexcept = default;

    ~DateTime() noexcept = default;

    Date GetDate() const noexcept;

    unsigned short GetYear() const noexcept;

    unsigned short GetMonth() const noexcept;

    unsigned short GetDay() const noexcept;

    Time GetTime() const noexcept;

    unsigned short GetHour() const noexcept;

    unsigned short GetMinute() const noexcept;

    unsigned short GetSecond() const noexcept;

    unsigned short GetMillisecond() const noexcept;

    DateTime &AddMillisecond() noexcept;

    static bool IsLeapYear(unsigned short year) noexcept;

    static DateTime Current(bool includeMillisecond = true) noexcept;

private:
    long long time_;
};

#endif //UTIL_DATETIME_H
