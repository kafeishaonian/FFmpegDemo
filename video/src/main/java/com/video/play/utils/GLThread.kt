package com.video.play.utils

import javax.microedition.khronos.egl.*


/**
 *
 * author: hongming.wei
 * data: 2022/1/21
 */
class GLThread : Thread {


    private var mWidth: Int = 0
    private var mHeight: Int = 0
    private var mRequestRender: Boolean = false
    private var mRenderMode: Int = 0
    private var mWantRenderNotification: Boolean = false
    private var mChangeSurface: Boolean = false

    private var mEGLConfigChooser: EGLConfigChooser? = null
    private var mEGLContextFactory: EGLContextFactory? = null
    private var mElgWindowSurfaceFactory: EGLWindowSurfaceFactory? = null
    private var mSurface: Any? = null
    private var mRenderer: GLViewRenderer? = null
    private var mEglContext: EglContextWrapper? = null


    constructor(
        configChooser: EGLConfigChooser,
        eglContextFactory: EGLContextFactory,
        elgWindowSurfaceFactory: EGLWindowSurfaceFactory,
        renderer: GLViewRenderer,
        renderMode: Int,
        surface: Any,
        sharedEglContext: EglContextWrapper
    ) : super() {
        mWidth = 0
        mHeight = 0
        mRequestRender = true
        mRenderMode = renderMode
        mWantRenderNotification = false

        mEGLConfigChooser = configChooser
        mEGLContextFactory = eglContextFactory
        mElgWindowSurfaceFactory = elgWindowSurfaceFactory
        mSurface = surface
        mRenderer = renderer
        mEglContext = sharedEglContext
    }


    fun setSurface(surface: Any) {
        if (mSurface != surface) {
            mChangeSurface = true
        }
        mSurface = surface
    }


    override fun run() {
        super.run()
        name = "GLThread $id"

        try {
            guardedRun()
        } catch (e: InterruptedException) {

        } finally {

        }
    }


    @Throws(InterruptedException::class)
    private fun guardedRun() {

    }


    interface EGLConfigChooser {

        fun chooseConfig(egl: EGL10, display: EGLDisplay): EGLConfig

        fun chooseConfig(
            display: android.opengl.EGLDisplay,
            recordable: Boolean
        ): android.opengl.EGLConfig
    }


    interface EGLContextFactory {
        fun createContext(
            egl: EGL10,
            display: EGLDisplay,
            eglConfig: EGLConfig,
            eglContext: EGLContext
        ): EGLContext

        fun destroyContext(egl: EGL10, display: EGLDisplay, context: EGLContext)

        fun createContextAPI17(
            display: android.opengl.EGLDisplay,
            eglConfig: android.opengl.EGLConfig,
            eglContext: android.opengl.EGLContext
        ): android.opengl.EGLContext

        fun destroyContextAPI17(
            display: android.opengl.EGLDisplay,
            context: android.opengl.EGLContext
        )
    }

    interface EGLWindowSurfaceFactory {
        /**
         * @return null if the surface cannot be constructed.
         */
        fun createWindowSurface(
            egl: EGL10,
            display: EGLDisplay,
            config: EGLConfig,
            nativeWindow: Any
        ): EGLSurface

        fun destroySurface(egl: EGL10, display: EGLDisplay, surface: EGLSurface)

        fun createWindowSurface(
            display: android.opengl.EGLDisplay,
            config: android.opengl.EGLConfig,
            nativeWindow: Any
        ): android.opengl.EGLSurface

        fun destroySurface(display: android.opengl.EGLDisplay, surface: android.opengl.EGLSurface)
    }


}