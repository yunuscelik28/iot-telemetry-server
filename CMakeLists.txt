cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(IoT_Telemetry)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mserver src/main_server.cpp src/server.cpp src/session.cpp)
target_include_directories(mserver PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)
add_executable(mclient client/main_client.cpp)
target_include_directories(mclient PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external
)

target_compile_definitions(mserver PRIVATE ASIO_STANDALONE)
target_compile_definitions(mclient PRIVATE ASIO_STANDALONE)

# Include necessary libraries (threads) for the project
find_package(Threads REQUIRED)

target_link_libraries(mserver PRIVATE Threads::Threads)
target_link_libraries(mclient PRIVATE Threads::Threads)

# Added to prevent networking errors on Windows
if(WIN32)
    target_link_libraries(mserver PRIVATE ws2_32 wsock32)
    target_link_libraries(mclient PRIVATE ws2_32 wsock32)

    target_compile_definitions(mserver PRIVATE _WIN32_WINNT=0x0A00)
    target_compile_definitions(mclient PRIVATE _WIN32_WINNT=0x0A00)
endif()
