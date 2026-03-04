#ifndef __LOG_H_
#define __LOG_H_

#include <stdio.h>

/* 日志级别 */
enum LOG_LEVEL
{
    LOG_LEVEL_OFF = 0,   /* 关闭日志输出 */
    LOG_LEVEL_FATAL = 1, /* 致命错误日志 */
    LOG_LEVEL_ERR = 2,   /* 错误日志 */
    LOG_LEVEL_WARN = 3,  /* 警告日志 */
    LOG_LEVEL_INFO = 4,  /* 信息日志 */
    LOG_LEVEL_DEBUG = 5, /* 调试日志 */
};

#define LOG_DEBUG /* 定义日志级别为调试级别，启用调试日志输出 */

/* 默认日志级别 */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG /* 如果没有显式定义日志级别，默认为调试级别 */
#endif

/* 日志颜色宏定义 */
#define COLOR_FATAL "\033[1;31m" /* 亮红色，用于致命错误日志 */
#define COLOR_ERROR "\033[0;31m" /* 红色，用于错误日志 */
#define COLOR_WARN "\033[0;33m"  /* 黄色，用于警告日志 */
#define COLOR_INFO "\033[0;32m"  /* 绿色，用于信息日志 */
#define COLOR_DEBUG "\033[0;36m" /* 青色，用于调试日志 */
#define COLOR_RESET "\033[0m"    /* 重置颜色，用于恢复默认颜色 */

/* 日志输出宏，只有在日志级别满足条件时才输出 */
#ifdef LOG_DEBUG
    #define LOG(level, format, ...) \
        do { \
            if (level <= LOG_LEVEL) { \
                const char *level_str = ""; /* 存储日志级别的字符串 */\
                const char *color = COLOR_RESET; /* 日志颜色的默认值为重置颜色 */ \
                switch (level) { \
                    case LOG_LEVEL_FATAL: level_str = "[F]"; color = COLOR_FATAL; break; /* 致命错误日志 */ \
                    case LOG_LEVEL_ERR: level_str = "[E]"; color = COLOR_ERROR; break; /* 错误日志 */ \
                    case LOG_LEVEL_WARN: level_str = "[W]"; color = COLOR_WARN; break; /* 警告日志 */ \
                    case LOG_LEVEL_INFO: level_str = "[I]"; color = COLOR_INFO; break; /* 信息日志 */ \
                    case LOG_LEVEL_DEBUG: level_str = "[D]"; color = COLOR_DEBUG; break; /* 调试日志 */ \
                    default: break; \
                } \
                printf("%s%s: ", color, level_str); /* 打印日志级别及其颜色 */ \
                printf(format, ##__VA_ARGS__); /* 打印日志内容 */ \
                printf("%s\r\n",COLOR_RESET); /* 输出完日志后重置颜色 */ \
            } \
        } while (0)
#else
    /* 如果没有定义 LOG_DEBUG，LOG 宏什么都不做 */
    #define LOG(level, format, ...) /* 不输出任何日志 */
#endif

#endif /* __LOG_H_ */
