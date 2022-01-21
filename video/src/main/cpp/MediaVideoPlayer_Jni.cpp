#include <jni.h>
#include <string>

static void MediaVideoPlayer_start(JNIEnv *env, jclass clazz){

}

static const JNINativeMethod gMethod[] = {
        {"start", "()V", (void *) MediaVideoPlayer_start}
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