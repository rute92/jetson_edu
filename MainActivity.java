package com.example.modulab.autocarcontrol;

import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.speech.RecognizerIntent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    //1 code is added -->
    private int task_state = 0;
    TextView mMainText;
    EditText mNumEdit1;
    TcpThread mThread;
    //1 code is added <--

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //1 code is added -->
        mMainText = (TextView)findViewById(R.id.connectText);
        mNumEdit1 = (EditText)findViewById(R.id.ipAddress);

        mThread = new TcpThread(mHandler);
        mThread.setDaemon(true);
        mThread.start();
        //1 code is added <--
    }

    //1 code is added -->
    public void onClickControl(View v) {
        Message msg;
        switch (v.getId()) {
            case R.id.bConnect:
                if(task_state == 1)  break;
                String addr = mNumEdit1.getText().toString();

                msg = new Message();
                msg.what = 0;
                msg.obj = (String)addr;
                mThread.mBackHandler.sendMessage(msg);
                break;

            case R.id.bDisconnect:
                if(task_state == 0)  break;
                msg = new Message();
                msg.what = 7;
                msg.obj = (String)mNumEdit1.getText().toString();
                mThread.mBackHandler.sendMessage(msg); // 작업스레드에 메세지를 던진다
                break;
        }
    }
    //1 code is added <--

    //1 code is added -->
    Handler mHandler = new Handler() {
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 0:
                    if(msg.arg1 == 0) {
                        mMainText.setText("connection successful : " + msg.obj);
                        task_state = 1;
                    }else if(msg.arg1 == 1)
                    {
                        mMainText.setText("connection fail : " + msg.obj);
                        task_state = 0;
                    }
                    break;

                case 7:
                    mMainText.setText("disconnect : ");
                    task_state = 0;
                    break;
               default:
                    break;
            }
        }
    };
}

//1 code is added <--
class TcpThread extends Thread {
    Handler mMainHandler;
    Handler mBackHandler;
    private Socket sock;
    private OutputStream outs;
    private int start = 0;

    TcpThread(Handler handler) {
        mMainHandler = handler;
    }

    public void run() {
        Looper.prepare(); // 작업스레드를 위한 looper 준비
        mBackHandler = new Handler() {
            public void handleMessage(Message msg) {
                Message retmsg = new Message();

                switch (msg.what) {
                    case 0:     // connect
                        try {
                            Thread.sleep(200);
                        } catch (InterruptedException e) {
                            ;
                        }
                        if (start == 0) {
                            try {
                                sock = new Socket((String) msg.obj, 1234);
                                outs = sock.getOutputStream();
                                start = 1;
                                // 쓰레드에서 main으로 메세지 전송 시작
                                retmsg.what = 0;
                                retmsg.arg1 = 0;                // success
                                retmsg.obj = (String) msg.obj;
                                // 쓰레드에서 main으로 메세지 전송 끝
                            } catch (Exception ex) {
                                ex.printStackTrace();

                                start = 0;
                                retmsg.what = 0;
                                retmsg.arg1 = 1;                // fail
                                retmsg.obj = (String) msg.obj;
                            }
                        }
                        break;

                    case 7:     // disconnect
                        try {
                            Thread.sleep(200);
                        } catch (InterruptedException e) {
                            ;
                        }
                        if (start == 1) {

                            try {

                            } catch (Exception ex) {
                                ex.printStackTrace();
                            }

                            try {
                                sock.close();
                                sock = null;
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                            start = 0;
                            // 쓰레드에서 main으로 메세지 전송 시작
                             retmsg.what = 7;
                            // 쓰레드에서 main으로 메세지 전송 끝
                        }
                        break;
                    default:
                        break;
                }
                mMainHandler.sendMessage(retmsg); // 결과값을 다시 메인으로 보내준다
            }
        };
        Looper.loop(); // 메세지 큐에서 메세지를 꺼내 핸들러로 전달한다.
    }
}
//1 code is added <--