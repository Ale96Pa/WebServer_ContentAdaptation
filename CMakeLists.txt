cmake_minimum_required(VERSION 3.9)
project(Server_ContentAdaptation C)

set(CMAKE_C_STANDARD 99)

include_directories(lib)
include_directories(/usr/include)
include_directories(/usr/include/ImageMagick-6)

add_executable(Server_ContentAdaptation http/http_request.c services/server_io.c services/server_io.h testServer.c services/adapt_image.c services/adapt_image.h http/http_response.c http/http_management.h test.c logging/logging.c logging/logging.h)