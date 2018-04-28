cmake_minimum_required(VERSION 3.9)
project(Server_ContentAdaptation C)

set(CMAKE_C_STANDARD 99)

include_directories(lib)
include_directories(/usr/include)
include_directories(/usr/include/ImageMagick-6)

add_executable(Server_ContentAdaptation main.c http_request.c server_io.c server_io.h testServer.c adapt_image.c adapt_image.h http_response.c http_management.h test.c logging.c logging.h)