/*************************************************************************
 * PLEASE SEE THE FILE "license.txt" (INCLUDED WITH THIS SOFTWARE PACKAGE)
 * FOR LICENSE AND COPYRIGHT INFORMATION.
 *************************************************************************/

/*************************************************************************
 *
 *  file:  misc.h
 *
 * =======================================================================
 */

#ifndef MISC_H_
#define MISC_H_

#include "portability.h"

#include <cinttypes>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <type_traits>

inline const char* get_directory_separator()
{
#ifdef WIN32
    return "\\";
#else //!WIN32
    return "/";
#endif
}

inline void normalize_separators(std::string& path)
{
    // Normalize separator chars.
    std::string::size_type j;
    while ((j = path.find('\\')) != std::string::npos)
    {
        path.replace(j, 1, "/");
    }
}

/* Should probably rename to avoid conflicts */
// Conversion of value to string
template<class T> std::string& to_string(const T& x, std::string& dest, int precision = 16, bool floatfixed = false)
{
    static std::ostringstream o;

    // get value into stream
    if (floatfixed)
    {
        o << std::fixed << std::setprecision(precision) << x;
    }
    else
    {
        o << std::setprecision(precision) << x;
    }

    dest.assign(o.str());
    o.str("");
    return dest;
}

// Conversion from string to value
template <class T> bool from_string(T& val, const std::string& str)
{
    std::istringstream i(str);
    i >> val;
    return !i.fail();
}

// compares two numbers stored as void pointers
// used for qsort calls
template <class T>
T compare_num(const void* arg1, const void* arg2)
{
    return *((T*) arg1) - *((T*) arg2);
}

template <class T> bool from_string(T& val, const char* const pStr)
{
    return from_string(val, std::string(pStr));
}

template <class T> inline T cast_and_possibly_truncate(void* ptr)
{
    return static_cast<T>(reinterpret_cast<uintptr_t>(ptr));
}

// Safer snprintf wrapper to avoid MSVC deprecation warning for _snprintf.
// Uses secure CRT on MSVC and falls back to standard vsnprintf elsewhere.
inline int safe_vsnprintf(char* buf, size_t bufsize, const char* fmt, va_list args)
{
    if (!buf || bufsize == 0)
    {
        return -1;
    }

#ifdef _MSC_VER
    // Use the secure CRT variant which avoids C4996 deprecation warnings.
    // _vsnprintf_s writes at most bufsize-1 characters and always null-terminates when _TRUNCATE is used.
    int ret = _vsnprintf_s(buf, bufsize, _TRUNCATE, fmt, args);
    if (ret < 0)
    {
        // Ensure termination on error/truncation
        buf[bufsize - 1] = '\0';
    }
    return ret;
#else
    int ret = vsnprintf(buf, bufsize, fmt, args);
    if (ret < 0)
    {
        // Ensure termination on platforms where vsnprintf may return negative on error.
        buf[bufsize - 1] = '\0';
    }
    else if (static_cast<size_t>(ret) >= bufsize)
    {
        // Ensure null-termination if truncated
        buf[bufsize - 1] = '\0';
    }
    return ret;
#endif
}

inline int safe_snprintf(char* buf, size_t bufsize, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = safe_vsnprintf(buf, bufsize, fmt, args);
    va_end(args);
    return ret;
}

// These functions have proven to be much faster than the c++ style ones above.
// TO
const size_t TO_C_STRING_BUFSIZE = 24; // uint64: 18446744073709551615 plus a few extra

//	Formats the 8 - bit signed integer v into the provided C buffer buf using safe_snprintf and the PRId8 
//  printf macro (so the correct platform - specific format is used).
inline const char* const to_c_string(const int8_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" PRId8, v);
    return buf;
}
inline const char* const to_c_string(const uint8_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" SCNu8, v);
    return buf;
}
inline const char* const to_c_string(const int16_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" SCNd16, v);
    return buf;
}
inline const char* const to_c_string(const uint16_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" SCNu16, v);
    return buf;
}
inline const char* const to_c_string(const int32_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" SCNd32, v);
    return buf;
}
inline const char* const to_c_string(const uint32_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" SCNu32, v);
    return buf;
}
inline const char* const to_c_string(const int64_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" SCNd64, v);
    return buf;
}
inline const char* const to_c_string(const uint64_t& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%" SCNu64, v);
    return buf;
}
inline const char* const to_c_string(const float& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%f", v);
    return buf;
}
inline const char* const to_c_string(const double& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%lf", v);
    return buf;
}
inline const char* const to_c_string(const long double& v, char* buf)
{
    safe_snprintf(buf, TO_C_STRING_BUFSIZE, "%Lf", v);
    return buf;
}

// Safer version of sscanf templates

/**
 * Parses an integral value of type T from a C string. Returns true on success, false on failure.
 * 
 * std::enable_if is used to ensure that this function is only instantiated for signed integral types.
 * i.e std::enable_if<Cond, R>::Type = R if Cond is true, otherwise it is not defined. This allows us to 
 * create a function that only works for signed integral types. Likewise for subsequent templates.
 */
template <typename T>
inline typename std::enable_if<std::is_integral<T>::value&& std::is_signed<T>::value, bool>::type
parse_from_c_string(const char* str, T& out)
{
    if (!str) return false;

    errno = 0;

    char* end = nullptr;
    
    long long tmp = std::strtoll(str, &end, 10);
    
    if (end == str || errno == ERANGE) return false;
    
	// Check if the parsed value fits within the range of T
    if (tmp < static_cast<long long>(std::numeric_limits<T>::min()) ||
        tmp > static_cast<long long>(std::numeric_limits<T>::max()))
    {
        return false;
    }
    
    out = static_cast<T>(tmp);
    
    return true;
}

/**
 * Parses an integral value of type T from a C string. Returns true on success, false on failure.
 * T could be any unsigned integral type (e.g., uint8_t, uint16_t, uint32_t, uint64_t).
 */
template <typename T>
inline typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value, bool>::type
parse_from_c_string(const char* str, T& out)
{
    if (!str) return false;

	// Setting errno to 0 before calling strtoull to detect overflow/underflow
    errno = 0;

    char* end = nullptr;

    unsigned long long tmp = std::strtoull(str, &end, 10);
    
	// Check if the conversion was successful and if the value is within the range of T
    if (end == str || errno == ERANGE) return false;
    
	// Check if the parsed value fits within the range of T
    if (tmp > static_cast<unsigned long long>(std::numeric_limits<T>::max()))
    {
        return false;
    }

	// Static cast instead of C-style cast for better safety and clarity
    // Becasue dynamic cast is not possible for primitive types.
    out = static_cast<T>(tmp);
    
    return true;
}

/**
 * Parses a floating-point value of type T from a C string. Returns true on success, false on failure.
 */
template <typename T>
inline typename std::enable_if<std::is_floating_point<T>::value, bool>::type
parse_from_c_string(const char* str, T& out)
{
    if (!str) return false;
    errno = 0;
    char* end = nullptr;

    if (std::is_same<T, float>::value)
    {
        float tmp = std::strtof(str, &end);
        if (end == str || errno == ERANGE) return false;
        
        out = static_cast<T>(tmp);

        return true;
    }

    if (std::is_same<T, double>::value)
    {
        double tmp = std::strtod(str, &end);
        if (end == str || errno == ERANGE) return false;

        out = static_cast<T>(tmp);
        
        return true;
    }
    
    // long double
    long double tmp = std::strtold(str, &end);
    if (end == str || errno == ERANGE) return false;

    out = static_cast<T>(tmp);
    
    return true;
}

// FROM
inline bool from_c_string(int8_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(uint8_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(int16_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(uint16_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(int32_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(uint32_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(int64_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(uint64_t& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(float& v, const char* const str)
{
    return parse_from_c_string(str, v);
}
inline bool from_c_string(double& v, const char* const str)
{
    return parse_from_c_string(str, v);
}

inline bool from_c_string(long double& v, const char* const str)
{
    return parse_from_c_string(str, v);
}

/** Casting between pointer-to-function and pointer-to-object is hard to do... legally
 *
 *  reinterpret_cast<...>(...) isn't actually capable of performing such casts in many
 *  compilers, complaining that it isn't ISO C++.
 *
 *  This function doesn't really do anything to guarantee that the provided types are
 *  pointers, so use it appropriately.
 */

template <typename Goal_Type>
struct Dangerous_Pointer_Cast
{
    template <typename Given_Type>
    static Goal_Type from(Given_Type given)
    {
        union
        {
            Given_Type given;
            Goal_Type goal;
        } caster;

        caster.given = given;

        return caster.goal;
    }
};

// To use the timer, call start, then stop. get_usec() will return the
// amount of time in the previous start-stop period.
//
// Calling start -> stop -> start -> get_usec is legal to minimize the
// amount of time the timer is not running.
class soar_timer
{
    public:
        soar_timer()
            : t1(0), elapsed(0), enabled_ptr(NULL)
        {
            raw_per_usec = get_raw_time_per_usec();
        }

        void set_enabled(bool* new_enabled)
        {
            enabled_ptr = new_enabled;
        }

        void start()
        {
            if ((!enabled_ptr) || (*enabled_ptr))
            {
                t1 = get_raw_time();
            }
        }

        void stop()
        {
            if ((!enabled_ptr) || (*enabled_ptr))
            {
                uint64_t t2 = get_raw_time();
                elapsed = t2 - t1;
            }
        }

        void reset()
        {
            t1 = elapsed = 0;
        }

        uint64_t get_usec()
        {
            if ((!enabled_ptr) || (*enabled_ptr))
            {
                return static_cast<uint64_t>(elapsed / raw_per_usec);
            }
            return 0;
        }

    private:
        uint64_t t1, elapsed;
        double raw_per_usec;
        bool* enabled_ptr;

        soar_timer(const soar_timer&);
        soar_timer& operator=(const soar_timer&);
};

// Utility class to be used with soar_timer instances, keeps track of multiple
// intervals and has some simple time conversions.
class soar_timer_accumulator
{
    private:
        uint64_t total;

    public:
        soar_timer_accumulator() : total(0) {}

        // Reset the accumulated time to zero.
        void reset()
        {
            total = 0;
        }

        // Add the timer's last interval to the accumulated time.
        void update(soar_timer& timer)
        {
            total += timer.get_usec();
        }

        // Return seconds as a double.
        double get_sec()
        {
            return total / 1000000.0;
        }

        // Return microseconds.
        uint64_t get_usec()
        {
            return total;
        }

        // Return milliseconds, truncated by integer division (not rounded).
        uint64_t get_msec()
        {
            return total / 1000;
        }
};

#endif /*MISC_H_*/

