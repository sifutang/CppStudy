package com.example.cplusplusstudy;

public class ThreadDemo {

    static {
        System.loadLibrary("native-lib");
    }

    private OnErrorListener mListener;

    public void setOnErrorListener(OnErrorListener listener) {
        mListener = listener;
    }

    public native void normalThread();

    public native void mutexThread();

    public native void callbackFromCPP();

    /**
     * Call from c++
     * @param code error code
     * @param msg  error mes
     */
    public void onError(int code, String msg) {
        if (mListener != null) {
            mListener.onError(code, msg);
        }
    }

    public interface OnErrorListener {
        void onError(int code, String msg);
    }
}
