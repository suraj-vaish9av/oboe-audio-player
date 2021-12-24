//
// Created by 43975 on 12/24/2021.
//
#ifndef OBOE_AUDIO_PLAYER_LOGGING_CPP
#define OBOE_AUDIO_PLAYER_LOGGING_CPP

#include "android/log.h"

#define APP_NAME "OboePlayer"

/**
 * A log util file where we create some macros for logging
 * although we can directly use __android_log_print from log.h but
 * it would be cumbersome for us to write it again-again with the arguments.
 *
 * So here prototype of __android_log_print is :
 * int __android_log_print(int prio, const char* tag, const char* fmt, ...)
 * arguments:
 * prio is priority which could be ANDROID_LOG_DEBUG, ANDROID_LOG_INFO, ANDROID_LOG_WARN, etc.
 * tag is the string or char array
 * fmt is format, means the where we define the format with string, something like this "value is: %d"
 * %d for int
 * %s for string
 * and so on.
 * ... is variable argument, the value of the variable argument will be substituted with format specifier,
 * means if provide integer value it will replace the %d according to the order.
 *
 * For example, To use this method we call it like this:
 * __android_log_print(ANDROID_LOG_DEBUG,"MyTAG","The message is= %s","This is first log");
 *
 * Here in the macros,
 * we have fixed the first two arguments that is priority and tag,
 * and the format and values, we can receive via var args means ... in macro
 *
 */

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, APP_NAME, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, APP_NAME, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__))

#endif //OBOE_AUDIO_PLAYER_LOGGING_CPP
