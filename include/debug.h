/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __UTILS__H__
#define __UTILS__H__

// #include <hTypes.h>

#if defined(__cplusplus) && !defined(FREERTOS_CONFIG_H)
#include <hSystem.h>

#ifndef STM32F4
#define LOG_FUNC(x...) fprintf(stderr, x)
#define LOG_FAIL_FUNC(x...) fprintf(stderr, x)
#define FAULT_HANDLER abort

#else
#define LOG_FUNC hFramework::sys.syslog
#define LOG_FAIL_FUNC hFramework::sys.fail_log
#define FAULT_HANDLER hFramework::sys.fault_handler
#endif

#else
int sys_syslog(const char* format, ...);
int fail_log(const char* format, ...);
void sys_fault_handler();
#define LOG_FUNC sys_syslog
#define LOG_FAIL_FUNC fail_log
#define FAULT_HANDLER sys_fault_handler
#endif

#define ASSERT(expr) \
		do { if (!(expr)) { \
			LOG_FAIL_FUNC("error: %s:%d\r\n", __FILE__,__LINE__); \
			FAULT_HANDLER(); } } while(0)

#define ASSERT2(expr,format,...) \
		do { if (!(expr)) { \
			LOG_FAIL_FUNC("error: %s:%d: " format "\r\n", __FILE__,__LINE__, ##__VA_ARGS__); \
			FAULT_HANDLER(); } } while(0)

#define TEXT(x) #x // makes DEFINE a string ("DEFINE")
#define TEXTOF(x) TEXT(x) // returns stringified value of x
#define _IS_DEBUG_ENABLED(x) (defined(x ## _DEBUG) || defined(DEBUG_ ## x))
#define IS_DEBUG_ENABLED(x) _IS_DEBUG_ENABLED(x) // we like macro expansion

#if defined(DEBUG) && !defined(NOLOG) && !defined(NODEBUG)
#  ifdef DEBUG_CATEGORY
#    ifndef DEBUG_TITLE
#      define DEBUG_TITLE TEXTOF(DEBUG_CATEGORY)
#    endif
// #    if IS_DEBUG_ENABLED(DEBUG_CATEGORY)
#      define LOG(x,...) LOG_FUNC("[" DEBUG_TITLE "] " x "\r\n", ##__VA_ARGS__)
#      define LOGnnl(x,...) LOG_FUNC("[" DEBUG_TITLE "] " x, ##__VA_ARGS__) // no new line
#      define LOGs(x,...) LOG_FUNC(x, ##__VA_ARGS__) // simple - no new line, no category title
#if defined(DEBUGIRQ)
#      define LOGIRQ(x,...) fail_log(x "\r\n", ##__VA_ARGS__)
#    endif
#  else
#    define LOG(x,...) LOG_FUNC(x "\r\n", ##__VA_ARGS__)
#    define LOGnnl(x,...) LOG_FUNC(x, ##__VA_ARGS__) // no new line
#    define LOGs(x,...) LOG_FUNC(x, ##__VA_ARGS__) // simple - no new line, no category title
#if defined(DEBUGIRQ)
#    define LOGIRQ(x,...) fail_log(x "\r\n", ##__VA_ARGS__)
#    endif
#  endif
#endif

#ifndef DEBUGIRQ
#  define LOGIRQ(x,...)
#endif

#ifdef DEBUG_CATEGORY
#  ifndef DEBUG_TITLE
#    define DEBUG_TITLE TEXTOF(DEBUG_CATEGORY)
#  endif
#  define SYSLOG(x,...) LOG_FUNC("[" DEBUG_TITLE "] " x "\r\n", ##__VA_ARGS__)
#  define SYSLOGnnl(x,...) LOG_FUNC("[" DEBUG_TITLE "] " x, ##__VA_ARGS__) // no new line
#  define SYSLOGs(x,...) LOG_FUNC(x, ##__VA_ARGS__) // simple - no new line, no category title
#else
#  define SYSLOG(x,...) LOG_FUNC(x "\r\n", ##__VA_ARGS__)
#  define SYSLOGnnl(x,...) LOG_FUNC(x, ##__VA_ARGS__) // no new line
#  define SYSLOGs(x,...) LOG_FUNC(x, ##__VA_ARGS__) // simple - no new line, no category title
#endif

#ifndef LOG
#define LOG(x,...)
#define LOGnnl(x,...)
#define LOGs(x,...)
#define LOGIRQ(x,...)
#endif

#endif /* __UTILS__H__ */
