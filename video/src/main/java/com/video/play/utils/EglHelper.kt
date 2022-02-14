package com.video.play.utils

import javax.microedition.khronos.egl.EGL10
import javax.microedition.khronos.egl.EGLContext
import javax.microedition.khronos.egl.EGLDisplay
import kotlin.RuntimeException

/**
 *
 * author: hongming.wei
 * data: 2022/1/21
 */
class EglHelper(
    val configChooser: GLThread.EGLConfigChooser,
    val eglContextFactory: GLThread.EGLContextFactory,
    val eglWindowSurfaceFactory: GLThread.EGLWindowSurfaceFactory
) : IEglHelper {

    private var mEgl: EGL10? = null
    private var mEglDisplay: EGLDisplay? = null


    override fun start(gelContext: EglContextWrapper): EglContextWrapper? {
        mEgl = EGLContext.getEGL() as EGL10?
        mEglDisplay = mEgl?.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY)

        if (mEglDisplay == EGL10.EGL_NO_DISPLAY) {
            throw RuntimeException("eglGetDisplay failed")
        }
        return null
    }

    override fun createSurface(surface: Any): Boolean {
        TODO("Not yet implemented")
    }

    override fun swap(): Int {
        TODO("Not yet implemented")
    }

    override fun destroySurface() {
        TODO("Not yet implemented")
    }

    override fun finish() {
        TODO("Not yet implemented")
    }

    override fun setPresentationTime(nsecs: Long) {
        TODO("Not yet implemented")
    }
}