package com.video.play.utils

/**
 *
 * author: hongming.wei
 * data: 2022/1/21
 */
interface GLViewRenderer {

    fun onSurfaceCreated()


    fun onSurfaceChanged(width: Int, height: Int)

    fun onDrawFrame()

}