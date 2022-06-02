//
// Created by MOMO on 2022/5/27.
//

#ifndef FFMPEGDEMO_LOGUTILS_H
#define FFMPEGDEMO_LOGUTILS_H


#include <android/log.h>

#define LOGCATE(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, FORMAT, ##__VA_ARGS__)




static long long GetSysCurrentTime(){
    struct timeval time;
    gettimeofday(&time, NULL);
    long long curTime = ((long long)(time.tv_sec))*1000+time.tv_usec/1000;
    return curTime;
}


#endif //FFMPEGDEMO_LOGUTILS_H
