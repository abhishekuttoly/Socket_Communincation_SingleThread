cmake_minimum_required(VERSION 3.10)   # Require at least CMake 3.10
project(MyProgram C)                   # Project name and language

# Find and link pthread properly
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/Application/Include
    ${CMAKE_SOURCE_DIR}/Server/Include
    ${CMAKE_SOURCE_DIR}/Log/Include
    ${CMAKE_SOURCE_DIR}/Parser/Include
)

set(SOURCES
    Server/Source/Server.c
    Application/Source/Application.c
    Log/Source/Log.c
    Parser/Source/Parser.c
)
# Add the executable from your source files
add_executable(ServerApplication main.c ${SOURCES})
target_link_libraries(ServerApplication Threads::Threads)