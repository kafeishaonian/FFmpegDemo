#include <jni.h>
#include "FFMediaPlayer.h"

#define LOGCATE(TAG, FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, FORMAT, ##__VA_ARGS__)
//static const char *TAG = "MediaVideoPlayer_JNI";


static jlong MediaVideoPlayer_Init(JNIEnv *env, jobject clazz, jstring pathUrl, jint renderType, jobject surface){

    const char * url = env->GetStringUTFChars(pathUrl, NULL);
    FFMediaPlayer *player = new FFMediaPlayer();
    player->Init(env, clazz, const_cast<char *>(url), renderType, surface);
    env->ReleaseStringUTFChars(pathUrl, url);
    return reinterpret_cast<jlong>(player);
}

static void MediaVideoPlayer_Play(JNIEnv *env, jobject thiz, jlong player_handle) {
    if (player_handle != 0) {
        FFMediaPlayer *ffMediaPlayer = reinterpret_cast<FFMediaPlayer *>(player_handle);
        ffMediaPlayer->Play();
    }
}



static const JNINativeMethod gMethod[] = {
        {"native_Init", "(Ljava/lang/String;ILandroid/view/Surface;)J", (void *) MediaVideoPlayer_Init},
        {"native_play", "(J)V", (void *) MediaVideoPlayer_Play}
};

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    jclass clazz = env->FindClass("com/video/play/MediaVideoPlayer");
    if (!clazz) {
        return -1;
    }

    if (env->RegisterNatives(clazz, gMethod, sizeof(gMethod) / sizeof(gMethod[0]))) {
        return -1;
    }

    return JNI_VERSION_1_6;
}