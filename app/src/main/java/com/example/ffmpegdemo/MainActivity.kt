package com.example.ffmpegdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import com.example.ffmpegdemo.databinding.ActivityMainBinding
import com.video.play.MediaVideoPlayer
import java.lang.ref.WeakReference

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = "哈哈哈哈"
        binding.sampleText.setOnClickListener {
            var a = MediaVideoPlayer()
            Log.e("TAG", "onCreate: -------> " )
//            a.start(WeakReference(this))
            a.getVideoAngle("")
        }
    }

}