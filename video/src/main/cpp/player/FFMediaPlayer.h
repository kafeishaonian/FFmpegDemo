//
// Created by MOMO on 2022/5/27.
//

#ifndef FFMPEGDEMO_FFMEDIAPLAYER_H
#define FFMPEGDEMO_FFMEDIAPLAYER_H

#include <jni.h>
#include "decoder/VideoDecoder.h"
#include "decoder/AudioDecoder.h"


class FFMediaPlayer {


public:
    FFMediaPlayer(){}
    ~FFMediaPlayer(){}

    void Init(JNIEnv *jniEnv, jobject obj, char *url, int videoRenderType, jobject surface);
    void Play();


private:
    JavaVM *m_JavaVM = nullptr;
    jobject m_JavaObj = nullptr;

    VideoDecoder *m_VideoDecoder = nullptr;
    AudioDecoder *m_AudioDecoder = nullptr;

    JavaVM *GetJavaVM();
};


#endif //FFMPEGDEMO_FFMEDIAPLAYER_H
