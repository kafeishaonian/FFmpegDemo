#!/bin/bash
set -x
# 目标Android版本
API=21

#===========================
#配置信息
# arm-v7a
  # CPU=armv7-a
  # ARCH=arm
  # ARCH_TYPE=armv7a-linux-androideabi
  # ARCH_CPU_TYPE=arm-linux-androideabi
  # PLATFORM_ARCH=arch-arm

# arm64
  CPU=armv8-a
  ARCH=arm64
  ARCH_TYPE=aarch64-linux-android
  ARCH_CPU_TYPE=aarch64-linux-android
  PLATFORM_ARCH=arch-arm64

# x86
#  CPU=x86
#  ARCH=x86
#  ARCH_TYPE=i686-linux-android
#  ARCH_CPU_TYPE=x86
#  PLATFORM_ARCH=arch-x86

#=============================

#so库输出目录
OUTPUT=/Users/momo/Desktop/android/FFmpeg/android/$CPU
# NDK的路径，根据自己的NDK位置进行设置
NDK=/Users/momo/Library/Android/sdk/ndk/21.0.6113669
# 编译工具链路径
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/darwin-x86_64
# 编译环境
SYSROOT=$TOOLCHAIN/sysroot

# 编译工具链路径
TOOLCHAIN_ARCH_PATH=$NDK/toolchains/$ARCH_CPU_TYPE-4.9/prebuilt/darwin-x86_64

PLATFORM=$NDK/platforms/android-21/$PLATFORM_ARCH

function build
{
  ./configure \
  --prefix=$OUTPUT \
  --target-os=android \
  --arch=$ARCH \
  --cpu=$CPU \
  --enable-asm \
  --enable-neon \
  --enable-cross-compile \
  --disable-small \
  --disable-shared \
  --enable-static \
  --disable-doc \
  --disable-ffplay \
  --disable-ffprobe \
  --disable-symver \
  --disable-ffmpeg \
  --sysroot=$SYSROOT \
  --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
  --cross-prefix-clang=$TOOLCHAIN/bin/$ARCH_TYPE$API- \
  --extra-cflags="-fPIC"

  make clean all
  # 这里是定义用几个CPU编译
  make -j12
  make install


  $TOOLCHAIN_ARCH_PATH/bin/$ARCH_CPU_TYPE-ld -rpath-link=$PLATFORM/usr/lib \
  -L$PLATFORM/usr/lib \
  -L$OUTPUT/lib \
  -soname libffmpeg.so -shared -nostdlib -Bsymbolic --whole-archive --no-undefined -o \
  $OUTPUT/lib/libffmpeg.so \
  $OUTPUT/lib/libavcodec.a \
  $OUTPUT/lib/libavdevice.a \
  $OUTPUT/lib/libavfilter.a \
  $OUTPUT/lib/libavformat.a \
  $OUTPUT/lib/libavutil.a \
  $OUTPUT/lib/libswresample.a \
  $OUTPUT/lib/libswscale.a \
  -lc -lm -lz -ldl -llog --dynamic-linker=/system/bin/linker \
  $TOOLCHAIN_ARCH_PATH/lib/gcc/$ARCH_CPU_TYPE/4.9.x/libgcc.a

}


build
