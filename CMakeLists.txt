cmake_minimum_required(VERSION 3.9)
project(Server_ContentAdaptation C)

set(CMAKE_C_STANDARD 99)

include_directories(lib)
include_directories(/usr/local/include)
include_directories(/usr/local/include/ImageMagick-7)

add_executable(Server_ContentAdaptation main.c http_request.h http_request.c server_io.c server_io.h testServer.c)