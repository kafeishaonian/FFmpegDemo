//
// Created by MOMO on 2022/5/28.
//

#ifndef FFMPEGDEMO_VIDEORENDER_H
#define FFMPEGDEMO_VIDEORENDER_H

#include "../../util/ImageDef.h"

#define VIDEO_RENDER_OPENGL 0

class VideoRender {

public:
    VideoRender(int type){
        m_RenderType = type;
    }
    virtual ~VideoRender(){}

    virtual void Init(int videoWidth, int videoHeight, int *dstSize) = 0;

    virtual void RenderVideoFrame(NativeImage *pImage) = 0;


private:
    int m_RenderType = VIDEO_RENDER_OPENGL;

};


#endif //FFMPEGDEMO_VIDEORENDER_H
