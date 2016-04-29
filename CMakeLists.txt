cmake_minimum_required(VERSION 3.2)
project(Thesis)

include_directories(/usr/include)
link_libraries(/usr/lib/libcrypto.a /usr/lib/libz.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -g")

set(SOURCE_FILES_COMMON Network/VerifiedObjectRequest.cpp Network/VerifiedObjectRequest.h HTTP/HttpConnection.cpp HTTP/HttpConnection.h HTTP/HttpRequest.cpp HTTP/HttpRequest.h HTTP/HttpResponse.cpp HTTP/HttpResponse.h Utilities/Log.cpp Utilities/Log.h TCP/Connection.cpp TCP/Connection.h TCP/ServerConnection.cpp TCP/ServerConnection.h PSR/PsrMessage.cpp PSR/PsrMessage.h Utilities/FileVerifier.cpp Utilities/FileVerifier.h Utilities/ConcurrentQueue.h Network/ObjectRequest.cpp Network/ObjectRequest.h)

set(SOURCE_FILES_PROXY proxyMain.cpp ${SOURCE_FILES_COMMON} Client/WebsiteDownloader.cpp Client/WebsiteDownloader.h Client/ProxyThread.cpp Client/ProxyThread.h)
add_executable(Proxy ${SOURCE_FILES_PROXY})

set(SOURCE_FILES_RESOLVER resolverMain.cpp ${SOURCE_FILES_COMMON} PSR/Resolver.cpp PSR/Resolver.h)
add_executable(Resolver ${SOURCE_FILES_RESOLVER})

set(SOURCE_FILES_FILESERVER fileServerMain.cpp ${SOURCE_FILES_COMMON} HTTP/FileServer.cpp HTTP/FileServer.h)
add_executable(FileServer ${SOURCE_FILES_FILESERVER})

file(COPY test DESTINATION . FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
file(COPY fileSigner.sh DESTINATION . FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)