package com.video.play

class MediaVideoPlayer {


    external fun start()

    companion object {
        init {
            System.loadLibrary("VideoPlayer")
        }
    }
}