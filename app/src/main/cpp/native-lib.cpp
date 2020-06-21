#include <jni.h>
#include <string>
#include <android/log.h>
#include <pthread.h>
#include <queue>
#include <unistd.h>

#include "JavaListener.h"

#define TAG "ThreadDemo"
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)

pthread_t thread;
void *normalCallback(void *data) {
    LOG_I("call back exe");
    pthread_exit(&thread);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_cplusplusstudy_ThreadDemo_normalThread(
        JNIEnv* env,
        jobject /* this */) {

    LOG_I("+++++++++++++++++ exec +++++++++++++++++++");
    pthread_create(&thread, NULL, normalCallback, NULL);
}


pthread_t product_thread;
pthread_t consume_thread;
pthread_mutex_t mutex;
pthread_cond_t cond;
std::queue<int> queue;
bool isEnd = false;

void *productCallback(void *data) {
    int i = 0;
    while (i < 5) {
        pthread_mutex_lock(&mutex);
        queue.push(1);
        LOG_I("product frame and notify consumer, size = %d, index = %d", queue.size(), i);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        i++;
        sleep(2); // 2s
    }

    LOG_E("exit product thread...");
    isEnd = true;
    // notify consume thread to exit
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    pthread_exit(&product_thread);
}

void *consumeCallback(void *data) {
    while (!isEnd) {
        pthread_mutex_lock(&mutex);
        if (!queue.empty()) {
            queue.pop();
            LOG_I("consume frame, size = %d", queue.size());
        } else {
            LOG_E("no data to consume...wait...");
            pthread_cond_wait(&cond, &mutex);
            LOG_E("no data to consume...wait done...");
        }
        pthread_mutex_unlock(&mutex);
        usleep(500); // 500ms
    }

    LOG_E("exit consume thread...");
    pthread_exit(&consume_thread);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cplusplusstudy_ThreadDemo_mutexThread(JNIEnv *env, jobject thiz) {

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);

    pthread_create(&product_thread, nullptr, productCallback, nullptr);
    pthread_create(&consume_thread, nullptr, consumeCallback, nullptr);

}

JavaVM *jvm;
JavaListener *javaListener;
pthread_t sub_thread;
void *subThreadCallback(void *data) {
    auto *listener = (JavaListener *)(data);
    listener->onError(1, 200, "error from c++ sub thread");
    delete javaListener;
    pthread_exit(&sub_thread);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cplusplusstudy_ThreadDemo_callbackFromCPP(JNIEnv *env, jobject thiz) {
    javaListener = new JavaListener(jvm, env, env->NewGlobalRef(thiz));
//    javaListener->onError(0, 100, "error from c++ main thread");
//    delete javaListener;
    pthread_create(&sub_thread, nullptr, subThreadCallback, javaListener);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm , void *reserved) {
    JNIEnv *env;
    jvm = vm;
    if (vm->GetEnv((void **)(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    return JNI_VERSION_1_6;
}