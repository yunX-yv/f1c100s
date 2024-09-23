
add_executable(chat main.cpp) 


list(APPEND EXTRA_LIBS json)
list(APPEND EXTRA_LIBS asr)

target_link_libraries(chat PUBLIC ${EXTRA_LIBS})


# target_include_directories(chat PUBLIC 
#                            "${PROJECT_BINARY_DIR}" 
#                         #    "${EXTRA_INCLUDES}" 
#                            ) 
include_directories(${PROJECT_SOURCE_DIR}/asr)






