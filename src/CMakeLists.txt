if(MSVC)
<<<<<<< HEAD
	set(EXE_TYPE WIN32)
	set(ADDITIONAL winmain.cpp)
=======
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS)
endif()

add_executable(App MACOSX_BUNDLE App.cpp App.h MainWindow.cpp MainWindow.h GLCanvas.cpp GLCanvas.h)
>>>>>>> bc699d018ab6c226315da0dd5f270afabfe01503

	add_custom_command(
		TARGET App POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_BUILD_DIR}/wxWidgets/lib/vc_x64_dll/wxbase330u_vc_x64_custom.dll 
				${CMAKE_BUILD_DIR}/src/Release/wxbase330u_vc_x64_custom.dll)

	add_custom_command(
		TARGET App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_BUILD_DIR}/wxWidgets/lib/vc_x64_dll/wxmsw330u_core_vc_x64_custom.dll 
				${CMAKE_BUILD_DIR}/src/Release/wxmsw330u_core_vc_x64_custom.dll)

	add_custom_command(
		TARGET App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_BUILD_DIR}/wxWidgets/lib/vc_x64_dll/wxmsw330u_gl_vc_x64_custom.dll 
				${CMAKE_BUILD_DIR}/src/Release/wxmsw330u_gl_vc_x64_custom.dll)

elseif(APPLE)
	set(EXE_TYPE MACOSX_BUNDLE)
endif()

add_executable(App ${EXE_TYPE} App.cpp App.h MainWindow.cpp MainWindow.h GLCanvas.cpp GLCanvas.h ${ADDITIONAL})

target_link_libraries(App wxgl)
