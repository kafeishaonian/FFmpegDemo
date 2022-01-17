package com.video.play

class NativeLib {

    /**
     * A native method that is implemented by the 'play' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'play' library on application startup.
        init {
            System.loadLibrary("play")
        }
    }
}