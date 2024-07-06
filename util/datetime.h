/**
 * @brief None
 * @file datetime.h
 * @note None
 * @date July 06 2024, 09:04
 * @author Escapist
 */

#ifndef UTIL_DATETIME_H
#define UTIL_DATETIME_H

#include <ctime>

struct Calendar {
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned short millisecond;
};

class TimeZone {
public:

    static long long Current() noexcept { return __timezone; }

private:

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

    static bool IsLeapYear(unsigned short year) noexcept;

    static DateTime TZCurrent(const TimeZone &timeZone, bool includeMillisecond = true) noexcept;

    static DateTime UTCCurrent(bool includeMillisecond = true) noexcept;

    static DateTime LocalCurrent(bool includeMillisecond = true) noexcept;

private:
    long long time_;
};

#endif //UTIL_DATETIME_H
