##########################################
#cmake 
##########################################
cmake_minimum_required(VERSION 2.80)

project(MP3Player)
include_directories(./include)

link_directories(./lib)

add_executable(echocloud_linux ./src/curl_get.c)
target_link_libraries(mp3_player curl)

