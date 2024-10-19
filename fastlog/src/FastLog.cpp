/*
 * Created by Steve Tchatchouang
 *
 * Copyright (c) 2022 All rights reserved
 */

#include <FastLog.h>
#include <cstdio>
#ifdef ANDROID
#include <android/log.h>
#endif
#include <cstdarg>

void FastLog::f(const char *file, int line, const char *message, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, message);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_ERROR, LIB_TAG, message, arg_ptr);
#else
    printf("\033[41m%s %-5s [%s@%d] ", current_time().c_str(), "FATAL", file, line);
    vprintf(message, arg_ptr);
    printf("\033[0m\n");
    fflush(stdout);
#endif
    va_end(arg_ptr);
}

void FastLog::e(const char *file, int line, const char *message, ...) {
#if defined(LOG_VERBOSE) || defined(LOG_INFO) || defined(LOG_WARN) || defined(LOG_ERROR)
    va_list arg_ptr;
    va_start(arg_ptr, message);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_ERROR, LIB_TAG, message, arg_ptr);
#else
    printf("\033[31m%s %-5s [%s@%d] ", current_time().c_str(), "ERROR", file, line);
    vprintf(message, arg_ptr);
    printf("\033[0m\n");
    fflush(stdout);
#endif
    va_end(arg_ptr);
#endif
}

void FastLog::w(const char *file, int line, const char *message, ...) {
#if defined(LOG_VERBOSE) || defined(LOG_INFO) || defined(LOG_WARN)
    va_list arg_ptr;
    va_start(arg_ptr, message);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_WARN, LIB_TAG, message, arg_ptr);
#else
    printf("\033[33m%s %-5s [%s@%d] ", current_time().c_str(), "WARN", file, line);
    vprintf(message, arg_ptr);
    printf("\033[0m\n");
    fflush(stdout);
#endif
    va_end(arg_ptr);
#endif
}

void FastLog::d(const char *file, int line, const char *message, ...) {
#if defined(LOG_VERBOSE)
    va_list arg_ptr;
    va_start(arg_ptr, message);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_DEBUG, LIB_TAG, message, arg_ptr);
#else
    printf("%s %-5s [%s@%d] ", current_time().c_str(), "DEBUG", file, line);
    vprintf(message, arg_ptr);
    printf("\n");
    fflush(stdout);
#endif
    va_end(arg_ptr);
#endif
}

void FastLog::i(const char *file, int line, const char *message, ...) {
#if defined(LOG_VERBOSE) || defined(LOG_INFO)
    va_list arg_ptr;
    va_start(arg_ptr, message);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_INFO, LIB_TAG, message, arg_ptr);
#else
    printf("\033[34m%s %-5s [%s@%d] ", current_time().c_str(), "INFO", file, line);
    vprintf(message, arg_ptr);
    printf("\033[0m\n");
    fflush(stdout);
#endif
    va_end(arg_ptr);
#endif
}