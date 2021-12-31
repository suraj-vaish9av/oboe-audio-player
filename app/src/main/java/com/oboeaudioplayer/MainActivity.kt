package com.oboeaudioplayer

import android.content.res.AssetManager
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        stringFromJNI()
        findViewById<Button>(R.id.btnPlay).setOnClickListener {
            startPlaying(assets,"sample.mp3");
        }
        findViewById<Button>(R.id.btnPause).setOnClickListener {
            stopPlaying()
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun startPlaying(assetManager: AssetManager, fileName:String);
    external fun stopPlaying();

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}