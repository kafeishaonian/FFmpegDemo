//
// Created by MOMO on 2022/5/27.
//

#include "DecoderBase.h"
#include "LogUtils.h"


int DecoderBase::Init(const char *url, AVMediaType mediaType) {
    LOGCATE(TAG, "DecoderBase::Init url=%s, mediaType=%d", url, mediaType);
    stpcpy(m_Url, url);
    m_MediaType = mediaType;
    return 0;
}

void DecoderBase::UnInit() {
    LOGCATE(TAG, "DecoderBase::UnInit m_MediaType=%d", m_MediaType);
}


int DecoderBase::InitFFDecoder() {
    int result = -1;
    do {
        m_AVFormatContext = avformat_alloc_context();

        if (avformat_open_input(&m_AVFormatContext, m_Url, NULL, NULL) != 0) {
            LOGCATE(TAG, "DecoderBase::InitFFDecoder avformat_open_input fail.");
            break;
        }

        if (avformat_find_stream_info(m_AVFormatContext, NULL) < 0) {
            LOGCATE(TAG, "DecoderBase::InitFFDecoder avformat_find_stream_info fail.");
            break;
        }

        for (int i = 0; i < m_AVFormatContext->nb_streams; i++) {
            if (m_AVFormatContext->streams[i]->codecpar->codec_type == m_MediaType) {
                m_StreamIndex = i;
                break;
            }
        }

        if (m_StreamIndex == -1) {
            LOGCATE(TAG, "DecoderBase::InitFFDecoder Fail to find stream index.");
            break;
        }

        //5.获取解码器参数
        AVCodecParameters *codecParameters = m_AVFormatContext->streams[m_StreamIndex]->codecpar;

        m_AVCodec = avcodec_find_decoder(codecParameters->codec_id);
        if (m_AVCodec == nullptr) {
            LOGCATE(TAG, "DecoderBase::InitFFDecoder avcodec_find_decoder fail.");
            break;
        }


        m_AVCodecContext = avcodec_alloc_context3(m_AVCodec);
        if (avcodec_parameters_to_context(m_AVCodecContext, codecParameters) != 0) {
            LOGCATE(TAG, "DecoderBase::InitFFDecoder avcodec_parameters_to_context fail.");
            break;
        }


        result = avcodec_open2(m_AVCodecContext, m_AVCodec, NULL);
        if (result < 0) {
            LOGCATE(TAG, "DecoderBase::InitFFDecoder avcodec_open2 fail. result=%d", result);
            break;
        }
        result = 0;

        m_Duration = m_AVFormatContext->duration / AV_TIME_BASE * 1000;
        m_Packet = av_packet_alloc();
        m_Frame = av_frame_alloc();

    } while (false);

    if (result != 0 && m_MsgContext && m_MsgCallback) {
        m_MsgCallback(m_MsgContext, MSG_DECODER_INIT_ERROR, 0);
    }
    return result;
}

void DecoderBase::UnInitDecoder() {
    LOGCATE(TAG, "DecoderBase::UnInitDecoder");

    if (m_Frame != nullptr) {
        av_frame_free(&m_Frame);
        m_Frame = nullptr;
    }

    if (m_Packet != nullptr) {
        av_packet_free(&m_Packet);
        m_Packet = nullptr;
    }

    if (m_AVCodecContext != nullptr) {
        avcodec_close(m_AVCodecContext);
        avcodec_free_context(&m_AVCodecContext);
        m_AVCodecContext = nullptr;
        m_AVCodec = nullptr;
    }

    if (m_AVFormatContext != nullptr) {
        avformat_close_input(&m_AVFormatContext);
        avformat_free_context(m_AVFormatContext);
        m_AVFormatContext = nullptr;
    }
}


void DecoderBase::Start() {
    if (m_Thread == nullptr){
        StartDecodingThread();
    } else {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        m_Cond.notify_all();
    }
}


void DecoderBase::DoAVDecoding(DecoderBase *decoder) {
    LOGCATE(TAG, "DecoderBase::DoAVDecoding");
    do {
        if (decoder->InitFFDecoder() != 0) {
            break;
        }
        decoder->OnDecoderReady();
        decoder->DecodingLoop();
    } while (false);

}

void DecoderBase::StartDecodingThread() {
    m_Thread = new std::thread(DoAVDecoding, this);
}

void DecoderBase::DecodingLoop() {
    LOGCATE(TAG, "DecoderBase::DecodingLoop start, m_MediaType=%d", m_MediaType);
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_DecoderState = STATE_DECODING;
        lock.unlock();
    }

    for (;;) {
        while (m_DecoderState == STATE_PAUSE) {
            std::unique_lock<std::mutex> lock(m_Mutex);
            LOGCATE(TAG, "DecoderBase::DecodingLoop waiting, m_MediaType=%d", m_MediaType);
            m_Cond.wait_for(lock, std::chrono::milliseconds(10));
            m_StartTimeStamp = GetSysCurrentTime() - m_CurTimeStamp;
        }

        if (m_DecoderState == STATE_STOP) {
            break;
        }

        if (m_StreamIndex == -1) {
            m_StartTimeStamp = GetSysCurrentTime();
        }

        if (DecodeOnePacket() != 0) {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_DecoderState = STATE_PAUSE;
        }
    }
    LOGCATE(TAG, "DecoderBase::DecodingLoop end");
}

int DecoderBase::DecodeOnePacket() {
    LOGCATE(TAG, "DecoderBase::DecodeOnePacket m_MediaType=%d", m_MediaType);
    if (m_SeekPosition > 0) {
        int64_t seek_target = static_cast<int64_t>(m_SeekPosition * 1000000);
        int64_t seek_min = INT64_MIN;
        int64_t seek_max = INT64_MAX;
        int seek_ret = avformat_seek_file(m_AVFormatContext, -1, seek_min, seek_target, seek_max, 0);
        if (seek_ret < 0) {
            m_SeekSuccess = false;
            LOGCATE(TAG, "BaseDecoder::DecodeOneFrame error while seeking m_MediaType=%d", m_MediaType);
        } else {
            if (-1 != m_StreamIndex) {
                avcodec_flush_buffers(m_AVCodecContext);
            }
            ClearCache();
            m_SeekSuccess = true;
            LOGCATE(TAG, "BaseDecoder::DecodeOneFrame seekFrame pos=%f, m_MediaType=%d", m_SeekPosition, m_MediaType);
        }
    }

    int result = av_read_frame(m_AVFormatContext, m_Packet);
    while (result == 0) {
        if (m_Packet->stream_index == m_StreamIndex) {

            if (avcodec_send_packet(m_AVCodecContext, m_Packet) == AVERROR_EOF) {
                result = -1;
                goto __EXIT;
            }

            int frameCount = 0;
            while (avcodec_receive_frame(m_AVCodecContext, m_Frame) == 0) {
                UpdateTimeStamp();
                AVSync();
                LOGCATE(TAG, "DecoderBase::DecodeOnePacket 000 m_MediaType=%d", m_MediaType);
                OnFrameAvailable(m_Frame);
                LOGCATE(TAG, "DecoderBase::DecodeOnePacket 0001 m_MediaType=%d", m_MediaType);
                frameCount++;
            }
            LOGCATE(TAG, "BaseDecoder::DecodeOneFrame frameCount=%d", frameCount);
            if (frameCount > 0) {
                result = 0;
                goto __EXIT;
            }
        }
        av_packet_unref(m_Packet);
        result = av_read_frame(m_AVFormatContext, m_Packet);
    }
    __EXIT:
    av_packet_unref(m_Packet);
    return result;
}

void DecoderBase::UpdateTimeStamp() {
    LOGCATE(TAG, "DecoderBase::UpdateTimeStamp");
    std::unique_lock<std::mutex> lock(m_Mutex);
    if (m_Frame->pkt_dts != AV_NOPTS_VALUE) {
        m_CurTimeStamp = m_Frame->pkt_dts;
    } else if (m_Frame->pts != AV_NOPTS_VALUE) {
        m_CurTimeStamp = m_Frame->pts;
    } else {
        m_CurTimeStamp = 0;
    }

    m_CurTimeStamp = static_cast<int64_t>((m_CurTimeStamp * av_q2d(m_AVFormatContext->streams[m_StreamIndex]->time_base)) * 1000);
    if (m_SeekPosition > 0 && m_SeekSuccess) {
        m_StartTimeStamp = GetSysCurrentTime() - m_CurTimeStamp;
        m_SeekPosition = 0;
        m_SeekSuccess = false;
    }
}

long DecoderBase::AVSync() {
    LOGCATE(TAG, "DecoderBase::AVSync");
    long curSysTime = GetSysCurrentTime();
    long elapsedTime = curSysTime - m_StartTimeStamp;

    if (m_MsgContext && m_MsgCallback && m_MediaType == AVMEDIA_TYPE_AUDIO) {
        m_MsgCallback(m_MsgContext, MSG_DECODING_TIME, m_CurTimeStamp * 1.0f / 1000);
    }

    long delay = 0;

    if (m_CurTimeStamp > elapsedTime) {
        auto sleepTime = static_cast<unsigned int> (m_CurTimeStamp - elapsedTime);
        sleepTime = sleepTime > DELAY_THRESHOLD? DELAY_THRESHOLD: sleepTime;
        av_usleep(sleepTime);
    }
    delay = elapsedTime - m_CurTimeStamp;
    return delay;
}



