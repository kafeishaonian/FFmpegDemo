//
// Created by MOMO on 2022/5/27.
//

#include "VideoDecoder.h"
#include "LogUtils.h"


void VideoDecoder::OnDecoderReady() {
    LOGCATE(TAG, "VideoDecoder::OnDecoderReady");

    m_VideoWidth = GetCodecContext()->width;
    m_VideoHeight = GetCodecContext()->height;

    if (m_MsgContext && m_MsgCallback) {
        m_MsgCallback(m_MsgContext, MSG_DECODER_READY, 0);
    }

    if (m_VideoRender != nullptr) {
        int dstSize[2] = {0};
        m_VideoRender->Init(m_VideoWidth, m_VideoHeight, dstSize);
        m_RenderWidth = dstSize[0];
        m_RenderHeight = dstSize[1];

        m_RGBAFrame = av_frame_alloc();
        int bufferSize = av_image_get_buffer_size(DST_PIXEL_FORMAT, m_RenderWidth, m_RenderHeight, 1);
        m_FrameBuffer = static_cast<uint8_t *>(av_malloc(bufferSize * sizeof(uint8_t)));
        av_image_fill_arrays(m_RGBAFrame->data, m_RGBAFrame->linesize, m_FrameBuffer, DST_PIXEL_FORMAT, m_RenderWidth, m_RenderHeight, 1);
        m_SwsContext = sws_getContext(m_VideoWidth, m_VideoHeight, GetCodecContext()->pix_fmt,
                                      m_RenderWidth, m_RenderHeight, DST_PIXEL_FORMAT,
                                      SWS_FAST_BILINEAR, NULL, NULL, NULL);
    } else {
        LOGCATE(TAG, "VideoDecoder::OnDecoderReady m_VideoRender == null");
    }
}

void VideoDecoder::OnFrameAvailable(AVFrame *frame) {
    LOGCATE(TAG, "VideoDecoder::OnFrameAvailable frame=%p", frame);
    if (m_VideoRender != nullptr && frame != nullptr) {
        NativeImage image;
        LOGCATE(TAG, "VideoDecoder::OnFrameAvailable frame[w,h]=[%d, %d],format=%d,[line0,line1,line2]=[%d, %d, %d]", frame->width, frame->height, GetCodecContext()->pix_fmt, frame->linesize[0], frame->linesize[1],frame->linesize[2]);
        if(false/*m_VideoRender->GetRenderType() == VIDEO_RENDER_ANWINDOW*/) {
//            sws_scale(m_SwsContext, frame->data, frame->linesize, 0,
//                      m_VideoHeight, m_RGBAFrame->data, m_RGBAFrame->linesize);
//
//            image.format = IMAGE_FORMAT_RGBA;
//            image.width = m_RenderWidth;
//            image.height = m_RenderHeight;
//            image.ppPlane[0] = m_RGBAFrame->data[0];
        } else if (GetCodecContext()->pix_fmt == AV_PIX_FMT_YUV420P || GetCodecContext()->pix_fmt == AV_PIX_FMT_YUVJ420P) {
            image.format = IMAGE_FORMAT_I420;
            image.width = frame->width;
            image.height = frame->height;
            image.pLineSize[0] = frame->linesize[0];
            image.pLineSize[1] = frame->linesize[1];
            image.pLineSize[2] = frame->linesize[2];
            image.ppPlane[0] = frame->data[0];
            image.ppPlane[1] = frame->data[1];
            image.ppPlane[2] = frame->data[2];
            if (frame->data[0] && frame->data[1] && !frame->data[2] && frame->linesize[0] == frame->linesize[1] && frame->linesize[2] == 0) {
                image.format = IMAGE_FORMAT_NV12;
            }
        } else if (GetCodecContext()->pix_fmt == AV_PIX_FMT_NV12) {
            image.format = IMAGE_FORMAT_NV12;
            image.width = frame->width;
            image.height = frame->height;
            image.pLineSize[0] = frame->linesize[0];
            image.pLineSize[1] = frame->linesize[1];
            image.ppPlane[0] = frame->data[0];
            image.ppPlane[1] = frame->data[1];
        } else if (GetCodecContext()->pix_fmt == AV_PIX_FMT_NV21) {
            image.format = IMAGE_FORMAT_NV21;
            image.width = frame->width;
            image.height = frame->height;
            image.pLineSize[0] = frame->linesize[0];
            image.pLineSize[1] = frame->linesize[1];
            image.ppPlane[0] = frame->data[0];
            image.ppPlane[1] = frame->data[1];
        } else if (GetCodecContext()->pix_fmt == AV_PIX_FMT_RGBA) {
            image.format = IMAGE_FORMAT_RGBA;
            image.width = frame->width;
            image.height = frame->height;
            image.pLineSize[0] = frame->linesize[0];
            image.ppPlane[0] = frame->data[0];
        } else {
            sws_scale(m_SwsContext, frame->data, frame->linesize, 0, m_VideoHeight, m_RGBAFrame->data, m_RGBAFrame->linesize);
            image.format = IMAGE_FORMAT_RGBA;
            image.width = m_RenderWidth;
            image.height = m_RenderHeight;
            image.ppPlane[0] = m_RGBAFrame->data[0];
        }

        m_VideoRender->RenderVideoFrame(&image);
    }



}
