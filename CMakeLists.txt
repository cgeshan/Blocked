set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.8)

project(Unoptimized)

set(CMAKE_OBJCXX_COMPILE_OBJECT "<CMAKE_OBJCXX_COMPILER> -x objective-c++ <DEFINES> <INCLUDES> <FLAGS> -c <SOURCE> -o <OBJECT>")

add_subdirectory(wxWidgets)
add_subdirectory(public)	
add_subdirectory(src)

enable_testing()