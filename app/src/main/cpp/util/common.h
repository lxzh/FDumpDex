#ifndef COMMON_H
#define COMMON_H

#include <jni.h>
#include <android/log.h>

#define TAG "DumpDex_Native"

#define LOG_LEVEL ANDROID_LOG_UNKNOWN
#define LOGV(FORMAT, ...) while(1){if(LOG_LEVEL<=ANDROID_LOG_VERBOSE){__android_log_print(ANDROID_LOG_VERBOSE, TAG, FORMAT, ##__VA_ARGS__);}break;};
#define LOGD(FORMAT, ...) while(1){if(LOG_LEVEL<=ANDROID_LOG_DEBUG){__android_log_print(ANDROID_LOG_DEBUG, TAG, FORMAT, ##__VA_ARGS__);}break;};
#define LOGI(FORMAT, ...) while(1){if(LOG_LEVEL<=ANDROID_LOG_INFO){__android_log_print(ANDROID_LOG_INFO, TAG, FORMAT, ##__VA_ARGS__);}break;};
#define LOGW(FORMAT, ...) while(1){if(LOG_LEVEL<=ANDROID_LOG_WARN){__android_log_print(ANDROID_LOG_WARN, TAG, FORMAT, ##__VA_ARGS__);}break;};
#define LOGE(FORMAT, ...) while(1){if(LOG_LEVEL<=ANDROID_LOG_ERROR){__android_log_print(ANDROID_LOG_ERROR, TAG, FORMAT, ##__VA_ARGS__);}break;};

#endif //COMMON_H
