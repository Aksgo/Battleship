include_directories(
    ${CMAKE_SOURCE_DIR}/include               
    ${CMAKE_CURRENT_SOURCE_DIR}/include       
)

add_library(Socket STATIC
    TcpServer.cpp
    BroadcastServer.cpp
)

add_executable(ServerTest
    server_test.cpp
)

add_executable(ClientTest
    client.cpp
)

target_link_libraries(ServerTest PRIVATE Socket ws2_32)

target_link_libraries(ClientTest PRIVATE ws2_32)