add_executable(menu MACOSX_BUNDLE menu.cpp)
add_executable(App MACOSX_BUNDLE App.cpp App.h MainWindow.cpp MainWindow.h GLCanvas.cpp GLCanvas.h)

target_link_libraries(menu wxgl)
target_link_libraries(App wxgl)

add_subdirectory(opengl)



