#pragma once

#include "Common.h"
#include <cstdio>
#include <cstdarg>
#include "Logger.h"
#include <sstream>
#include <stdarg.h>

#include <ctime>
#include <iomanip>

#define PiPo_BREAK

#define PiPo_ASSERT_ALL

#ifdef PiPo_ASSERT_ALL
    #define PiPo_ASSERT(cond, msg, ...) \
        do \
        { \
            if (!(cond)) \
            { \
                Picayune_Porter::Assert::ReportFailure(#cond, __FILE__, __LINE__, (msg), ##__VA_ARGS__); \
                PiPo_BREAK \
            } \
        } while(0)

    #define PiPo_ASSERT_WARNING(cond, msg, ...) \
        do \
        { \
            if (!(cond)) \
            { \
                Picayune_Porter::Assert::ReportFailure(#cond, __FILE__, __LINE__, (msg), ##__VA_ARGS__); \
            } \
        } while(0)
#else
    #define PiPo_ASSERT(cond, msg, ...) 
#endif

namespace Picayune_Porter
{
    namespace Assert
    {
        void ShutDown();

        extern std::string lastErrorMessage;

        const std::string currentDateTime();

        void ReportFailure(const char * condition, const char * file, int line, const char * msg, ...);
    }
}
