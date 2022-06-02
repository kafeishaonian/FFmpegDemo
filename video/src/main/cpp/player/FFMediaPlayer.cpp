//
// Created by MOMO on 2022/5/27.
//

#include "FFMediaPlayer.h"



void FFMediaPlayer::Init(JNIEnv *jniEnv, jobject obj, char *url, int videoRenderType,
                         jobject surface) {

    jniEnv->GetJavaVM(&m_JavaVM);
    m_JavaObj = jniEnv->NewGlobalRef(obj);

    m_VideoDecoder = new VideoDecoder(url);
//    m_AudioDecoder = new AudioDecoder(url);

//    m_VideoDecoder->SetVideoRender(VideoRender::)




}


JavaVM * FFMediaPlayer::GetJavaVM() {
    return m_JavaVM;
}

void FFMediaPlayer::Play() {

    if (m_VideoDecoder) {
        m_VideoDecoder->Start();
    }



}
