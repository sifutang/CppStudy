# CppStudy
ndk c++ study demo

## C++创建子线程
使用POSIX线程库，头文件<pthread.h>

```
// define
pthread_t thread;
// create
pthread_create(&thread, NULL, callback, NULL);
// release
pthread_exit(&thread);
```

### 线程锁对象
```
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);
pthread_mutex_destroy(&mutex);

pthread_mutex_lock
pthread_mutex_unlock
```

### 线程条件对象
```
pthread_cond_t cond;
pthread_cond_init(&cond, NULL);
pthread_cond_destroy(&cond);

pthread_cond_signal: 用于发出条件信号
pthread_cond_wait: 用于线程阻塞等待
```

## JavaVM 和 JniEnv
JavaVM是进程中唯一的，JniEnv是和线程相关的，一个java线程对应一个JniEnv
在C++子线程中要想方法jni方法，必须通过JVM拿到env对象

获取JVM对象： 
```
JNI_OnLoad(JavaVM* vm,void* reserved)
```
通过JVM获取JniEnv：
```
JNIEnv *env;  
jvm->AttachCurrentThread(&env, 0);
jvm->DetachCurrentThread();
```

