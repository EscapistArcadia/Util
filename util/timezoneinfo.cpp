/**
 * @brief None
 * @file timezoneinfo.cpp
 * @note None
 * @date July 06 2024, 18:56
 * @author Escapist
 */

#include "datetime.h"

static const long long kOffsetFactor = 3600 >> 2;

const char_t *kTimeZoneNameBegin/*[2]*/[27] = {
        STR("UTC-12"), STR("UTC-11"), STR("UTC-10"), STR("UTC-9"), STR("UTC-8"), STR("UTC-7"),
        STR("UTC-6"), STR("UTC-5"), STR("UTC-4"), STR("UTC-3"), STR("UTC-2"), STR("UTC-1"), STR("UTC"),
        STR("UTC+1"), STR("UTC+2"), STR("UTC+3"), STR("UTC+4"), STR("UTC+5"), STR("UTC+6"), STR("UTC+7"),
        STR("UTC+8"), STR("UTC+9"), STR("UTC+10"), STR("UTC+11"), STR("UTC+12"), STR("UTC+13"), STR("UTC+14")
};

const char_t **kTimeZoneName = kTimeZoneNameBegin + 12;

const char_t *TimeZoneInfo::GetName() const noexcept {
    return kTimeZoneName[offset_ / 3600];
}

long long TimeZoneInfo::GetOffset() const noexcept {
    return offset_;
}

TimeZoneInfo &TimeZoneInfo::Set(TimeZone &tz) noexcept {
    offset_ = (long long) tz * 3600 >> 2;
    return *this;
}

TimeZoneInfo TimeZoneInfo::Local() noexcept {
#if defined(OS_UNIX)
#elif defined(OS_LINUX)
    tzset();
    return TimeZoneInfo(-__timezone - __daylight * 60 * 60);
#elif defined(OS_WINDOWS)
#elif defined(OS_MACOS)
#endif
}
