//
// Created by MOMO on 2022/5/29.
//

#ifndef FFMPEGDEMO_VIDEOGLRENDER_H
#define FFMPEGDEMO_VIDEOGLRENDER_H

#include <thread>
#include "ImageDef.h"
#include <GLES3/gl3.h>
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <vec2.hpp>
#include <gtc/matrix_transform.hpp>

#include "render/BaseGLRender.h"
#include "VideoRender.h"

#define MATH_PI 3.1415926535897932384626433832802

class VideoGLRender : public VideoRender, public BaseGLRender{

public:

    static VideoGLRender *GetInstance();

    virtual void Init(int videoWidth, int videoHeight, int *dstSize);

    virtual void RenderVideoFrame(NativeImage *pImage);

    virtual void UpdateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY);

    virtual void UpdateMVPMatrix(TransformMatrix * pTransformMatrix);

private:
    static std::mutex m_Mutex;
    static VideoGLRender* s_Instance;
    int m_FrameIndex;
    glm::mat4 m_MVPMatrix;
    NativeImage m_RenderImage;


    VideoGLRender();
    virtual ~VideoGLRender();



};


#endif //FFMPEGDEMO_VIDEOGLRENDER_H
