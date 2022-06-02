package com.video.play

import android.view.Surface

class MediaVideoPlayer {




    companion object {
        init {
            System.loadLibrary("VideoPlayer")
        }
    }






    private external fun native_Init(
        url: String,
        videoRenderType: Int,
        surface: Surface?
    ): Long


    private external fun native_play(playerHandle: Long)
}