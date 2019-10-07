package com.example.wavemaker;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    //all these functions have been defined in native-lib , we declare them here to use in main activity
    private native void touchEvent(int action);
    private native void startEngine();
    private native void stopEngine();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        startEngine();  //call native function to start engine when activity is created
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    @Override
    public boolean onTouchEvent(MotionEvent motionEvent){
        //receives all touch events and passes them to the native void function defined in jni bridge
        touchEvent(motionEvent.getAction());
        return super.onTouchEvent(motionEvent);
    }
    @Override
    public void onDestroy(){
        stopEngine(); //call native function to stop engine when activity is destroyed
        super.onDestroy();
    }
}
