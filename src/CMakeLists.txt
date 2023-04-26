if(MSVC)
	message(STATUS "Windows OS Found")
	set(EXE_TYPE WIN32)

elseif(APPLE)
	message(STATUS "MAC OSX Found")
	set(EXE_TYPE MACOSX_BUNDLE)

elseif(UNIX)
	message(STATUS "UNIX OS Found")
	set(EXE_TYPE)

endif()

# Supress depricated OpenGL warnings
add_definitions(-w)

# Create executable for main GUI
add_executable(App ${EXE_TYPE} 	App.h App.cpp 
								MainWindow.h MainWindow.cpp 
								GLCanvas.h GLCanvas.cpp
								Block.h Block.cpp
								cost.h cost.cpp
								Layer.h Layer.cpp 
								Merge.h Merge.cpp 
								WorkerThread.h WorkerThread.cpp
								Stability.h Stability.cpp
								Voxelize.h Voxelize.cpp)

target_link_libraries(App wxgl ysclass)


if(MSVC)
	message(STATUS "App built successfully, copying dynamic link libraries...")
	add_custom_command(
		TARGET App POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${CMAKE_BINARY_DIR}/wxWidgets/lib/vc_x64_dll/wxbase330u_vc_x64_custom.dll
				${CMAKE_BINARY_DIR}/wxWidgets/lib/vc_x64_dll/wxmsw330u_core_vc_x64_custom.dll
				${CMAKE_BINARY_DIR}/wxWidgets/lib/vc_x64_dll/wxmsw330u_gl_vc_x64_custom.dll
				$<TARGET_FILE_DIR:App>
	)
endif()

# Copying sample stl files
message(STATUS "Copying sample STL files to build folder...")
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/)

# Unit Testing 
add_subdirectory(tests)

				
				
