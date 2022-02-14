package com.video.play.utils

/**
 *
 * author: hongming.wei
 * data: 2022/1/21
 */
interface IEglHelper {

    fun start(gelContext: EglContextWrapper): EglContextWrapper?

    fun createSurface(surface: Any): Boolean

    fun swap(): Int

    fun destroySurface()

    fun finish()

    fun setPresentationTime(nsecs: Long)


}