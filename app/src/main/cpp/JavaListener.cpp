//
// Created by alaiye on 6/21/20.
//

#include "JavaListener.h"
#include <android/log.h>

#define TAG "JavaListener"
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)

JavaListener::~JavaListener() {
    LOG_I("release java listener start");

    if (type == 0) {
        jniEnv->DeleteGlobalRef(jobj);
    } else {
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, 0);
        if (env == nullptr) {
            return;
        }
        env->DeleteGlobalRef(jobj);
        jvm->DetachCurrentThread();
    }

    LOG_I("release java listener end");
}

JavaListener::JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj) {
    jvm = vm;
    jniEnv = env;
    jobj = obj;

    jclass clz = env->GetObjectClass(jobj);
    if (clz == nullptr) {
        return;
    }

    jmid = env->GetMethodID(clz, "onError", "(ILjava/lang/String;)V");
}

void JavaListener::onError(int threadType, int code, const char *msg) {
    type = threadType;
    if (threadType == 0) {
        // main
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, jmid, code, jmsg);
        jniEnv->DeleteLocalRef(jmsg);
    } else if (threadType == 1) {
        // sub-thread
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, 0);
        if (env == nullptr) {
            return;
        }

        jstring jmsg = env->NewStringUTF(msg);
        env->CallVoidMethod(jobj, jmid, code, jmsg);
        env->DeleteLocalRef(jmsg);

        jvm->DetachCurrentThread();
    }
}
