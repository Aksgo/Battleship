include_directories(../include)
include_directories(include)

add_library(Socket STATIC
    broadcastServer.cpp
    client.cpp
    include/TcpServer.h TcpServer.cpp
    include/BroadcastServer.h BroadcastServer.cpp
)

add_executable(ServerTest
    server_test.cpp
)

target_link_libraries(ServerTest PRIVATE
    Socket
)