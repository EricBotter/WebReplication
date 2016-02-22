cmake_minimum_required(VERSION 3.3)
project(Thesis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES_PROXY proxyMain.cpp HTTP/HttpRequest.cpp HTTP/HttpRequest.h HTTP/HttpResponse.cpp HTTP/HttpResponse.h Utilities/Log.cpp Utilities/Log.h TCP/Connection.cpp TCP/Connection.h TCP/ServerConnection.cpp TCP/ServerConnection.h)
add_executable(Proxy ${SOURCE_FILES_PROXY})

set(SOURCE_FILES_RESOLVER resolverMain.cpp HTTP/HttpRequest.cpp HTTP/HttpRequest.h HTTP/HttpResponse.cpp HTTP/HttpResponse.h Utilities/Log.cpp Utilities/Log.h TCP/Connection.cpp TCP/Connection.h TCP/ServerConnection.cpp TCP/ServerConnection.h)
add_executable(Resolver ${SOURCE_FILES_RESOLVER})
