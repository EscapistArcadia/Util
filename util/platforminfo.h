/**
 * @brief None
 * @file platforminfo.h
 * @note None
 * @date July 06 2024, 18:56
 * @author Escapist
 */

#ifndef UTIL_PLATFORMINFO_H
#define UTIL_PLATFORMINFO_H

#if defined(__linux__) || defined(__linux)
#define OS_LINUX
#elif defined(__unix__) || defined(__unix)
#define OS_UNIX
#elif defined(_WIN32)
#define OS_WINDOWS
#elif defined(__APPLE__)
#define OS_APPLE
#endif

#if defined(__x86_64__)
#define ARCH_x86_64
#elif defined(__i386__)
#define ARCH_x86
#elif defined(__aarch64__)
#define ARCH_ARM64
#elif defined(__arm__)
#define ARCH_ARM
#elif defined(__mips__)
#define ARCH_MIPS
#ifdef __mips64__
#define ARCH_64BITS
#else
#define ARCH_32BITS
#endif
#ifdef __MIPSEB__
#define ARCH_BIG_ENDIAN
#elif defined(__MIPSEL__)
#define ARCH_LITTLE_ENDIAN
#endif
#endif

#if defined(ARCH_x86_64) || defined(ARCH_ARM64)
#define ARCH_64BITS
#elif defined(ARCH_x86) || defined(ARCH_ARM)
#define ARCH_32BITS
#else
#endif

#if defined(ARCH_x86_64) || defined(ARCH_ARM64) || defined(ARCH_x86) || defined(ARCH_ARM)
#define ARCH_LITTLE_ENDIAN
#else
#endif

#ifdef ARCH_64BITS
#define STR(str) L##str
using char_t = wchar_t;
#else
#define STR(str) str
using char_t = char;
#endif

enum class OperatingSystem {
    Invalid,
    Unix,
    Linux,
    Windows,
    MacOS
};

enum class Architecture {

};

class PlatformInfo {
public:
private:
    OperatingSystem os_;
    Architecture arch_;

};

#endif //UTIL_PLATFORMINFO_H
