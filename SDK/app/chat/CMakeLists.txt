cmake_minimum_required(VERSION 3.10) 

# 设定工程名和版本号 
project(chat VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

set(CMAKE_C_COMPILER "/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin/arm-buildroot-linux-gnueabi-g++")

set(CAMKE_SYSTEM_NAME Linux)

# 设置编译器搜索路径
SET(TOOLCHAIN_DIR "/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/bin")
set(CMAKE_FIND_ROOT_PATH "/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot" 
"/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/include" 
"/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/lib")
# set(CMAKE_SYSROOT /opt/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi/)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -lpthread -lm")

# # f1c200s
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft")


link_directories(/home/xiao/f1c100s/buildroot-2021.02.4/output/host/arm-buildroot-linux-gnueabi/lib)
link_libraries(pthread)


# 链接 curl库
link_directories(/home/xiao/project/f1c100s/SDK/app/3rd_lib/curl-8.0.1/_install/lib)
include_directories(/home/xiao/project/f1c100s/SDK/app/3rd_lib/curl-8.0.1/_install/include)
link_libraries(libcurl.a)

# 链接 openssl curl库需要
# link_directories(/home/hah/Downloads/openssl-3.1.0/build/install/lib)
# link_libraries(ssl)
# link_libraries(crypto)

# 链接 zlib curl库需要
link_directories(~/project/f1c100s/SDK/app/3rd_lib/zlib-1.2.11/_install/lib)
link_libraries(libz.a)


# 链接 paho mqtt
#link_directories(./paho-mqtt-c-1.3.12/lib)
#include_directories(./paho-mqtt-c-1.3.12/include)
#link_libraries(libpaho-mqtt3c.a)

# 链接 pocketsphinx
link_directories(./pocketsphinx/lib)
include_directories(./pocketsphinx/include)
include_directories(./pocketsphinx/examples/include)
link_libraries(libpocketsphinx.a)
# link_libraries(/home/hah/Desktop/chat/pocketsphinx/lib/libm.a)

add_subdirectory(tts)
add_subdirectory(json)
add_subdirectory(asr)
add_subdirectory(src)
add_subdirectory(audiorecorder)
add_subdirectory(mqtt-client)

