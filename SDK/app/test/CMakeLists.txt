cmake_minimum_required(VERSION 3.10) 

# 设定工程名和版本号 
project(chat VERSION 1.0) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

set(CMAKE_C_COMPILER "/home/xiao/project/buildroot-2021.02.4/output/host/bin/arm-buildroot-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "/home/xiao/project/buildroot-2021.02.4/output/host/bin/arm-buildroot-linux-gnueabi-g++")

set(CMAKE_CROSSCOMPILING TRUE)
set(CAMKE_SYSTEM_NAME Linux)

set(CMAKE_BUILD_TOOL "make")
# 设置编译器搜索路径
# set(CMAKE_FIND_ROOT_PATH /home/xiao/f1c100s/tools/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi)
# set(CMAKE_SYSROOT /home/xiao/f1c100s/tools/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabi)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#set(CMAKE_VERBOSE_MAKEFILE ON)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -lm ")
#set(CMAKE_EXE_LINKER_FLAGS "-static")

# # f1c200s
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft")


#link_directories(/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/lib)
#include_directories(/home/xiao/f1c100s/tools/arm-buildroot-linux-gnueabi_sdk-buildroot/include)
#link_directories(/home/xiao/project/f1c100s/SDK/root/rootfs/lib)
#link_libraries(pthread)
link_libraries(c)


# 链接 curl库

link_directories(/home/xiao/project/f1c100s/SDK/app/3rd_lib/curl-8.0.1/_install/lib)
include_directories(/home/xiao/project/f1c100s/SDK/app/3rd_lib/curl-8.0.1/_install/include)
link_libraries(/home/xiao/project/f1c100s/SDK/app/3rd_lib/curl-8.0.1/_install/lib/libcurl.a -lpthread)

link_directories(/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/_install/lib)
link_libraries(/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/_install/lib/libssl.a)
link_libraries(/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/_install/lib/libcrypto.a)
link_libraries(dl)
link_libraries(atomic)

# 链接 zlib curl库需要
#link_directories(~/project/f1c100s/SDK/app/3rd_lib/zlib-1.2.11/_install/lib)
#link_libraries(libz.a)

add_subdirectory(lib)
add_subdirectory(src)
