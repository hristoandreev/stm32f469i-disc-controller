cmake_minimum_required(VERSION 3.6)

add_library(cjson STATIC
    cJSON.c
)

target_include_directories(cjson
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
