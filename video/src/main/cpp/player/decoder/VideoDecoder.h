//
// Created by MOMO on 2022/5/27.
//

#ifndef FFMPEGDEMO_VIDEODECODER_H
#define FFMPEGDEMO_VIDEODECODER_H

extern "C" {
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/jni.h>
};

#include "DecoderBase.h"
#include "render/video/VideoRender.h"


class VideoDecoder : public DecoderBase{

public:
    VideoDecoder(char *url){
        Init(url, AVMEDIA_TYPE_VIDEO);
    }

    virtual ~VideoDecoder(){
        UnInit();
    }

    void SetVideoRender(VideoRender *videoRender) {
        m_VideoRender = videoRender;
    }


private:
    VideoRender *m_VideoRender = nullptr;

    const AVPixelFormat DST_PIXEL_FORMAT = AV_PIX_FMT_RGBA;
    int m_VideoWidth = 0;
    int m_VideoHeight = 0;

    int m_RenderWidth = 0;
    int m_RenderHeight = 0;

    AVFrame *m_RGBAFrame = nullptr;
    uint8_t *m_FrameBuffer = nullptr;

    SwsContext *m_SwsContext = nullptr;

    virtual void OnDecoderReady();
    virtual void OnFrameAvailable(AVFrame *frame);


};


#endif //FFMPEGDEMO_VIDEODECODER_H
