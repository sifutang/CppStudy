package com.example.cplusplusstudy;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "MainActivity";

    private ThreadDemo mThreadDemo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.sample_btn).setOnClickListener(this);
        mThreadDemo = new ThreadDemo();
    }


    @Override
    public void onClick(View v) {
//        mThreadDemo.normalThread();
//        mThreadDemo.mutexThread();

        mThreadDemo.callbackFromCPP();
        mThreadDemo.setOnErrorListener(new ThreadDemo.OnErrorListener() {
            @Override
            public void onError(int code, String msg) {
                Log.d(TAG, "onError: code = " + code + ", msg = " + msg);
            }
        });
    }
}
