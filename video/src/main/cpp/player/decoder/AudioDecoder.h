//
// Created by MOMO on 2022/5/28.
//

#ifndef FFMPEGDEMO_AUDIODECODER_H
#define FFMPEGDEMO_AUDIODECODER_H


#include "DecoderBase.h"

class AudioDecoder : public DecoderBase{


public:
    AudioDecoder(char *url){
        Init(url, AVMEDIA_TYPE_AUDIO);
    }

    virtual ~AudioDecoder(){
        UnInit();
    }


};


#endif //FFMPEGDEMO_AUDIODECODER_H
