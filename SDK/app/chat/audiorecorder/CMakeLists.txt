add_library(audio_rec audio_rec.cpp)


# 代码中想要调用MathFunctions中的函数，就需要#include对应的代码路径，
# 然而MathFunctions本身不需要#include，所以这里采用INTERFACE来满足该需求，
# 将MathFunctions中的CMakeLists.txt更改为
# 同时我们可以将上层目录中的CMakeLists.txt中的EXTRA_INCLUDE变量去掉，
# 同时将其从target_include_directories中删除
target_include_directories(audio_rec INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/json) 

link_libraries(pthread)

