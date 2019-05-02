##########################################
#cmake 
##########################################
cmake_minimum_required(VERSION 2.80)

project(MP3Player)
include_directories(./include)

link_directories(./lib)

add_executable(mp3_player ./src/main.c ./src/curl_get.c)
target_link_libraries(mp3_player -lpthread -lcurl -lz)
#add_executable(mp3_player ./src/minimad.c)
#target_link_libraries(mp3_player -lpthread -lcurl -lz  asound -lmad)


