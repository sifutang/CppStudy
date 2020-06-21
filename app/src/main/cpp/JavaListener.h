//
// Created by alaiye on 6/21/20.
//


#include <jni.h>

#ifndef CPLUSPLUSSTUDY_JAVALISTENER_H
#define CPLUSPLUSSTUDY_JAVALISTENER_H

class JavaListener {

public:
    JavaVM *jvm;
    _JNIEnv *jniEnv;
    jobject jobj;

    jmethodID jmid;
    jint type;

public:
    JavaListener(JavaVM *vm, _JNIEnv *env, jobject obj);
    ~JavaListener();

    /**
     *
     * @param threadType 0 - main, 1 - sub-thread
     * @param code error code
     * @param msg  error msg
     */
    void onError(int threadType, int code, const char *msg);
};


#endif //CPLUSPLUSSTUDY_JAVALISTENER_H
