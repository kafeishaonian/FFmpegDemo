//
// Created by MOMO on 2022/5/29.
//

#include "VideoGLRender.h"


VideoGLRender* VideoGLRender::s_Instance = nullptr;
std::mutex VideoGLRender::m_Mutex;


VideoGLRender::VideoGLRender():VideoRender(VIDEO_RENDER_OPENGL) {

}


VideoGLRender *VideoGLRender::GetInstance() {
    if(s_Instance == nullptr) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if(s_Instance == nullptr) {
            s_Instance = new VideoGLRender();
        }
    }
    return s_Instance;
}

void VideoGLRender::Init(int videoWidth, int videoHeight, int *dstSize) {
    LOGCATE(TAG, "VideoGLRender::InitRender video[w, h]=[%d, %d]", videoWidth, videoHeight);
    if (dstSize != nullptr) {
        dstSize[0] = videoWidth;
        dstSize[1] = videoHeight;
    }
    m_FrameIndex = 0;
    UpdateMVPMatrix(0, 0, 1.0f, 1.0f);
}

void VideoGLRender::RenderVideoFrame(NativeImage *pImage) {
    LOGCATE(TAG, "VideoGLRender::RenderVideoFrame pImage=%p", pImage);
    if (pImage == nullptr || pImage->ppPlane[0] == nullptr) {
        return;
    }
    std::unique_lock<std::mutex> lock(m_Mutex);
    if (pImage->width != m_RenderImage.width || pImage->height != m_RenderImage.height) {
        if (m_RenderImage.ppPlane[0] != nullptr){
            NativeImageUtil::FreeNativeImage(&m_RenderImage);
        }
        memset(&m_RenderImage, 0, sizeof(NativeImage));
        m_RenderImage.format = pImage->format;
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        NativeImageUtil::AllocNativeImage(&m_RenderImage);
    }
    NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);

}



VideoGLRender::~VideoGLRender() {

}

void VideoGLRender::UpdateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY) {
    angleX = angleX % 360;
    angleY = angleY % 360;
    //转化为弧度角
    float radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);
    //投影矩阵
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    //视图矩阵
    glm::mat4 view = glm::lookAt(
            glm::vec3(0, 0, 4),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0));
    //模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.0f));
    model = glm::rotate(model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    m_MVPMatrix = projection * view * model;
}

void VideoGLRender::UpdateMVPMatrix(TransformMatrix *pTransformMatrix) {
    BaseGLRender::UpdateMVPMatrix(pTransformMatrix);
}
