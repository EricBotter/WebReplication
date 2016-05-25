cmake_minimum_required(VERSION 3.2)
project(Thesis)

include_directories(/usr/include)
link_libraries(/usr/lib/libcrypto.a /usr/lib/libz.a)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -pthread -g")

set(SOURCE_FILES_COMMON Client/WebsiteDownloader.cpp Client/WebsiteDownloader.h Network/VerifiedObjectRequest.cpp Network/VerifiedObjectRequest.h HTTP/HttpClientConnection.cpp HTTP/HttpClientConnection.h HTTP/HttpRequest.cpp HTTP/HttpRequest.h HTTP/HttpResponse.cpp HTTP/HttpResponse.h Utilities/Log.cpp Utilities/Log.h TCP/Connection.cpp TCP/Connection.h TCP/ServerConnection.cpp TCP/ServerConnection.h PSR/PsrMessage.cpp PSR/PsrMessage.h Network/FileVerifier.cpp Network/FileVerifier.h Utilities/ConcurrentQueue.h Network/ObjectRequest.cpp Network/ObjectRequest.h Utilities/IniReader.cpp Utilities/IniReader.h)

set(SOURCE_FILES_PROXY proxyMain.cpp ${SOURCE_FILES_COMMON} Client/ProxyThread.cpp Client/ProxyThread.h)
add_executable(Proxy ${SOURCE_FILES_PROXY})

set(SOURCE_FILES_RESOLVER resolverMain.cpp ${SOURCE_FILES_COMMON} PSR/Resolver.cpp PSR/Resolver.h)
add_executable(Resolver ${SOURCE_FILES_RESOLVER})

set(SOURCE_FILES_FILESERVER fileServerMain.cpp ${SOURCE_FILES_COMMON} Server/FileServer.cpp Server/FileServer.h Server/ServerDownloader.cpp Server/ServerDownloader.h)
add_executable(FileServer ${SOURCE_FILES_FILESERVER})

file(COPY test DESTINATION . FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
file(COPY fileSigner.sh DESTINATION . FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)