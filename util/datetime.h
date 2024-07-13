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
#include <type_traits>
#include <cstdint>

#ifdef ARCH_64BITS
using Int64 = long long;
using UInt64 = unsigned long long;
#else
class Int64 {
public:
    Int64() noexcept {}

    Int64(const Int64 &other) noexcept = default;

    Int64(long high, unsigned long low) noexcept: high_(high), low_(low) {}

    explicit Int64(long from) noexcept: high_(0), low_(from) {
        if (low_ < 0) {
            high_ = -1;
        }
    }

    ~Int64() noexcept = default;

    void ToHex(char_t buf[17]) {
        static const char_t kHexMap[16] = {
                STR('0'), STR('1'), STR('2'), STR('3'), STR('4'), STR('5'), STR('6'), STR('7'),
                STR('8'), STR('9'), STR('A'), STR('B'), STR('C'), STR('D'), STR('E'), STR('F'),
        };
        buf[0] = kHexMap[(high_ & 0xF0000000) >> 28];
        buf[1] = kHexMap[(high_ & 0x0F000000) >> 24];
        buf[2] = kHexMap[(high_ & 0x00F00000) >> 20];
        buf[3] = kHexMap[(high_ & 0x000F0000) >> 16];
        buf[4] = kHexMap[(high_ & 0x0000F000) >> 12];
        buf[5] = kHexMap[(high_ & 0x00000F00) >> 8];
        buf[6] = kHexMap[(high_ & 0x000000F0) >> 4];
        buf[7] = kHexMap[(high_ & 0x0000000F)];
        buf[8] = kHexMap[(low_ & 0xF0000000) >> 28];
        buf[9] = kHexMap[(low_ & 0x0F000000) >> 24];
        buf[10] = kHexMap[(low_ & 0x00F00000) >> 20];
        buf[11] = kHexMap[(low_ & 0x000F0000) >> 16];
        buf[12] = kHexMap[(low_ & 0x0000F000) >> 12];
        buf[13] = kHexMap[(low_ & 0x00000F00) >> 8];
        buf[14] = kHexMap[(low_ & 0x000000F0) >> 4];
        buf[15] = kHexMap[(low_ & 0x0000000F)];
        buf[16] = char_t(0);
    }

    long GetHigh() const noexcept { return high_; }

    unsigned long GetLow() const noexcept { return low_; }

    inline Int64 &operator+=(const Int64 &operand) noexcept {
        unsigned long low = low_;                   // records the previous value
        high_ += operand.high_;
        low_ += operand.low_;
        if (low_ < low || low_ < operand.low_) {    // checks overflow
            ++high_;
        }
        return *this;
    }

    inline Int64 &operator+=(long operand) noexcept {
        unsigned long low = low_;
        low_ += operand;
        if (operand < 0) {                          // borrow
            --high_;
        }
        if (low_ < low || low_ < (unsigned long) operand) {
            ++high_;
        }
        return *this;
    }

    inline Int64 operator+(const Int64 &operand) noexcept {
        return Int64(*this).operator+=(operand);
    }

    inline Int64 operator+(long operand) noexcept {
        return Int64(*this).operator+=(operand);
    }

    inline Int64 &operator++() {
        if (++low_ == 0) {
            ++high_;
        }
        return *this;
    }

    inline Int64 operator++(int) {
        Int64 other(*this);
        operator++();
        return other;
    }

    inline Int64 operator-() const noexcept {
        Int64 neg(~high_, (~low_) + 1);
        if (neg.low_ == 0) {
            ++neg.high_;
        }
        return neg;
    }

    inline Int64 &operator-=(const Int64 &operand) noexcept {
        unsigned long low = low_;                   // records the previous value
        high_ -= operand.high_;
        low_ -= operand.low_;
        if (low_ > low || low_ > operand.low_) {    // checks overflow
            --high_;
        }
        return *this;
    }

    inline Int64 &operator-=(long operand) noexcept {
        unsigned long low = low_;
        low_ -= operand;
        if (operand < 0) {                          // borrow
            ++high_;
        }
        if (low_ < low || low_ < (unsigned long) operand) {
            --high_;
        }
        return *this;
    }

    inline Int64 operator-(const Int64 &operand) noexcept {
        return Int64(*this).operator-=(operand);
    }

    inline Int64 operator-(long operand) noexcept {
        return Int64(*this).operator-=(operand);
    }

    inline Int64 &operator--() {
        if (~(--low_) == -1) {                              // checks overflow
            --high_;
        }
        return *this;
    }

    inline Int64 operator--(int) {
        Int64 other(*this);
        operator--();
        return other;
    }
private:
    long high_;
    unsigned long low_;
};
#endif

/**
 * \c QTimeZone represents a specific time zone for some specific region(s).
 * It determines the offset between local time and the UTC/GMT time, and whether
 * the time zone is in daylight savings.
 */
class TimeZone {
public:
    enum class TZ {
        UTC_N12 = -(12 << 2), UTC_N11 = -(11 << 2), UTC_N10 = -(10 << 2), UTC_N09 = -(9 << 2), UTC_N08 = -(8 << 2),
        UTC_N07 = -(7 << 2), UTC_N06 = -(6 << 2), UTC_N05 = -(5 << 2), UTC_N04 = -(4 << 2), UTC_N03 = -(3 << 2),
        UTC_N02 = -(2 << 2), UTC_N01 = -(1 << 2),
        UTC = 0,
        UTC_01 = 1 << 2, UTC_02 = 2 << 2, UTC_03 = 3 << 2, UTC_04 = 4 << 2, UTC_05 = 5 << 2, UTC_06 = 6 << 2,
        UTC_07 = 7 << 2, UTC_08 = 8 << 2, UTC_09 = 9 << 2, UTC_10 = 10 << 2, UTC_11 = 11 << 2, UTC_12 = 12 << 2,
        UTC_13 = 13 << 2, UTC_14 = 14 << 2
    };

    TimeZone() = delete;

    /**
     * Creates a \c TimeZone instance for a specific time zone in the world.
     * @param tz the specified time zone in the world
     */
    TimeZone(TZ tz) noexcept;

    /**
     * Creates a \c TimeZone instance with specified UTC offset and daylight saving offset.
     * @param offset the offset between desired local time and the UTC time.
     * @param daylightOffset extra daylight saving time
     */
    TimeZone(long offset, bool dls) noexcept: offset_(offset), dls_(dls) {}

    ~TimeZone() = default;

    /**
     * @return the offset of the time zone.
     */
    long GetOffset() const noexcept {
        return offset_;
    }


private:
    long offset_;       /* the offset between the local time and the UTC/GMT time */
    bool dls_;          /* true if the offset has daylight*/
};

/**
 * \c QDate represents a particular day, as year, month, day.
 */
class Date {
public:
    Date() noexcept: year_(-1), month_(-1), day_(-1) {}

    Date(short year, short month, short day);

    Date(const Date &other) noexcept = default;

    ~Date() = default;

    short GetYear() const noexcept {
        return year_;
    }

    short GetMonth() const noexcept {
        return month_;
    }

    short GetDay() const noexcept {
        return day_;
    }

    inline short GetDayOfWeek() const noexcept;

    inline short GetDayOfYear() const noexcept;

    inline Date &SetDate(short year, short month, short day) noexcept;

    inline Date &AddYear(short year);

    inline Date &AddMonth(short month);

    inline Date &AddDay(short day);

    inline Date &SubtractYear(short year);

    inline Date &SubtractMonth(short month);

    inline Date &SubtractDay(short day);

    static inline bool IsLeapYear(short year) noexcept {
        return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
    }

private:
    short year_;
    short month_;
    short day_;
};

class Time {
public:
    Time() : hour_(-1), minute_(-1), second_(-1) {}

    Time(short hour, short minute, short second) : hour_(hour), minute_(minute), second_(second) {}

    Time(const Time &other) noexcept = default;

    ~Time() noexcept = default;

    short GetHour() const noexcept {
        return hour_;
    }

    short GetMinute() const noexcept {
        return minute_;
    }

    short GetSecond() const noexcept {
        return second_;
    }

    Time &SetHour(short hour);

    Time &SetMinute(short minute);

    Time &SetSecond(short second);

private:
    short hour_;
    short minute_;
    short second_;
};

class Calendar {

private:
    short year_;
    short month_;
    short day_;
    short hour_;
    short minute_;
    short second_;

    friend class DateTime;
};


class DateTime {
public:
    DateTime() noexcept: time_(-1) {}

    DateTime(const DateTime &other) noexcept = default;

    explicit DateTime(time_t time) noexcept: time_(time) {}

    explicit DateTime(const tm &tm) noexcept
            : DateTime(tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec) {}

    explicit DateTime(const Calendar &cal) noexcept
            : DateTime(cal.year_, cal.month_, cal.day_, cal.hour_, cal.minute_, cal.second_) {}

    DateTime(short year, short month, short day, short hour, short minute, short second);

    ~DateTime() noexcept = default;

    Calendar ToCalendar() const noexcept;

    Date GetDate() const noexcept;

    Time GetTime() const noexcept;

    short GetYear() const noexcept;

    short GetMonth() const noexcept;

    short GetDay() const noexcept;

    short GetHour() const noexcept;

    short GetMinute() const noexcept;

    short GetSecond() const noexcept;

    DateTime &SetDate(const Date &date) noexcept;

    DateTime &SetTime(const Time &time) noexcept;

    DateTime &AddSeconds(Int64 seconds) noexcept;

    DateTime &AddMinutes(Int64 minutes) noexcept;

    DateTime &AddHours(Int64 hours) noexcept;

    DateTime &AddDays(Int64 days) noexcept;

    DateTime &AddMonths(Int64 months) noexcept;

    DateTime &AddYears(Int64 years);

private:
    Int64 time_;
};

class TimeSpan {
public:
    TimeSpan() noexcept = default;

    TimeSpan(const TimeSpan &other) noexcept = default;

    short GetDay() const noexcept;

    short GetHour() const noexcept;

    Int64 GetTotalHour() const noexcept;

    short GetMinute() const noexcept;

    Int64 GetTotalMinute() const noexcept;

    short GetSecond() const noexcept;

    Int64 GetTotalSecond() const noexcept;

    TimeSpan &AddSeconds(Int64 seconds) noexcept;

    TimeSpan &AddMinutes(Int64 minutes) noexcept;

    TimeSpan &AddHours(Int64 hours) noexcept;

    TimeSpan &AddDays(Int64 days) noexcept;

private:
    Int64 diff_;
};

#endif //UTIL_DATETIME_H
