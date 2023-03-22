if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS)
endif()

add_executable(App MACOSX_BUNDLE App.cpp App.h MainWindow.cpp MainWindow.h GLCanvas.cpp GLCanvas.h)

target_link_libraries(App wxgl)



