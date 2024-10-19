//
// Created by Steve Tchatchouang on 04/10/2022.
//

#ifndef LIB_FASTLOG_H
#define LIB_FASTLOG_H

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>

class FastLog {
public:
    static void f(const char* file, int line, const char *message, ...) __attribute__((format (printf, 3, 4)));
    static void e(const char* file, int line, const char *message, ...) __attribute__((format (printf, 3, 4)));
    static void w(const char* file, int line, const char *message, ...) __attribute__((format (printf, 3, 4)));
    static void d(const char* file, int line, const char *message, ...) __attribute__((format (printf, 3, 4)));
    static void i(const char* file, int line, const char *message, ...) __attribute__((format (printf, 3, 4)));

private:
    static std::string current_time(std::string const &format = "%F %T"){
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        std::stringstream ss;
        ss << std::put_time(&tm, format.c_str());
        return ss.str();
    }
};
//#ifdef ENABLE_LOG
#define DEBUG_E(message, ...) FastLog::e(__FILE__, __LINE__, message, ##__VA_ARGS__)
#define DEBUG_W(message, ...) FastLog::w(__FILE__, __LINE__, message, ##__VA_ARGS__)
#define DEBUG_D(message, ...) FastLog::d(__FILE__, __LINE__, message, ##__VA_ARGS__)
#define DEBUG_I(message, ...) FastLog::i(__FILE__, __LINE__, message, ##__VA_ARGS__)
#define DEBUG_F(message, ...) FastLog::f(__FILE__, __LINE__, message, ##__VA_ARGS__)
//#else
//#define DEBUG_E(message, ...)
//#define DEBUG_W(message, ...)
//#define DEBUG_D(message, ...)
//#define DEBUG_I(message, ...)
//#define DEBUG_F(message, ...)
//#endif

#define LOGE DEBUG_E
#define LOGD DEBUG_D
#define LOGI DEBUG_I
#define LOGF DEBUG_F
#define LOGW DEBUG_W
#endif //LIB_FASTLOG_H
