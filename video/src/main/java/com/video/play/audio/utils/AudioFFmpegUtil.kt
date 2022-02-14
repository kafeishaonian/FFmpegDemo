package com.video.play.audio.utils

/**
 *
 * author: hongming.wei
 * data: 2022/1/21
 */
class AudioFFmpegUtil {


    companion object {
        //音频转码
        fun transformAudio(srcFile: String, targetFile: String): List<String> {
            var transformAudioCmd = "ffmpeg -i %s %s"
            transformAudioCmd = String.format(transformAudioCmd, srcFile, targetFile)
            return transformAudioCmd.split(" ")
        }

        fun cutAudio(srcFile: String, startTime: Int, duration: Int, targetFile: String) : List<String> {
            var cutAudioCmd = "ffmpeg -i %s -ss %d -t %d %s";
            cutAudioCmd = String.format(cutAudioCmd, srcFile, startTime, duration, targetFile)
            return cutAudioCmd.split(" ")
        }
    }


}