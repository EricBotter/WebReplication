cmake_minimum_required(VERSION 3.3)
project(Thesis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g")

set(SOURCE_FILES_COMMON HTTP/HttpRequest.cpp HTTP/HttpRequest.h HTTP/HttpResponse.cpp HTTP/HttpResponse.h Utilities/Log.cpp Utilities/Log.h TCP/Connection.cpp TCP/Connection.h TCP/ServerConnection.cpp TCP/ServerConnection.h PSR/PsrMessage.cpp PSR/PsrMessage.h Utilities/FileVerifier.cpp Utilities/FileVerifier.h Utilities/ConcurrentQueue.h Utilities/Lockable.h Network/NetworkRequest.cpp Network/NetworkRequest.h)

set(SOURCE_FILES_PROXY proxyMain.cpp ${SOURCE_FILES_COMMON} Client/WebsiteDownloader.cpp Client/WebsiteDownloader.h Client/ProxyThread.cpp Client/ProxyThread.h)
add_executable(Proxy ${SOURCE_FILES_PROXY})

set(SOURCE_FILES_RESOLVER resolverMain.cpp ${SOURCE_FILES_COMMON} PSR/Resolver.cpp PSR/Resolver.h)
add_executable(Resolver ${SOURCE_FILES_RESOLVER})

set(SOURCE_FILES_FILESERVER fileServerMain.cpp ${SOURCE_FILES_COMMON} HTTP/FileServer.cpp HTTP/FileServer.h)
add_executable(FileServer ${SOURCE_FILES_FILESERVER})

set(SOURCE_FILES_TEST test/testMain.cpp ${SOURCE_FILES_COMMON})
add_executable(Test ${SOURCE_FILES_TEST})

set(SOURCE_FILES_SIGNER fileSigner.cpp ${SOURCE_FILES_COMMON})
add_executable(FileSigner ${SOURCE_FILES_SIGNER})
