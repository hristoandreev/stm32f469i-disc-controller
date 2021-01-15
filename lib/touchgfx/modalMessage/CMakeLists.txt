cmake_minimum_required(VERSION 3.6)

add_library(modal_message STATIC
    modalMessage.cpp
)

target_include_directories(modal_message
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
