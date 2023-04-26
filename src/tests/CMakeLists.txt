#------------------------------------------- Unit Tests (organized by component) -------------------------------------------#

# Set executable type based on operating system
if(MSVC)
	set(EXE_TYPE WIN32)

elseif(APPLE)
	set(EXE_TYPE MACOSX_BUNDLE)

elseif(UNIX)
	set(EXE_TYPE)

endif()

#------------------------------------------- GUI Tests -------------------------------------------#
add_executable(BuildTest ${EXE_TYPE} 	../App.h ../App.cpp 
										../MainWindow.h ../MainWindow.cpp 
										../GLCanvas.h ../GLCanvas.cpp
										../Block.h ../Block.cpp
										../cost.h ../cost.cpp
										../Layer.h ../Layer.cpp 
										../Merge.h ../Merge.cpp 
										../WorkerThread.h ../WorkerThread.cpp
										../Stability.h ../Stability.cpp
										../Voxelize.h ../Voxelize.cpp
										)

target_link_libraries(BuildTest wxgl ysclass)
add_test(NAME BuildTest COMMAND cmake --build . --config Release --target BuildTest)


add_executable(RenderSTLTest ${EXE_TYPE} RenderSTLTest.cpp)
add_test(NAME RenderSTLTestBuild COMMAND cmake --build . --config Release --target RenderSTLTest)
add_test(NAME RenderSTLTest COMMAND RenderSTLTest)


add_executable(ExportBoMTest ${EXE_TYPE} BoMTest.cpp
										 )

target_link_libraries(ExportBoMTest)
add_test(NAME ExportBoMTestBuild COMMAND cmake --build . --config Release --target ExportBoMTest)
add_test(NAME ExportBoMTest COMMAND ExportBoMTest)

add_executable(ImportTest ${EXE_TYPE} ImportTest.cpp
										 )

target_link_libraries(ImportTest)
add_test(NAME ImportTestBuild COMMAND cmake --build . --config Release --target ImportTest)
add_test(NAME ImportTest COMMAND ImportTest)



# Cost  Tests

#------------------------------------------- Stability Tests -------------------------------------------#

add_executable(emptyLayerStabilityTest ${EXE_TYPE} emptyLayerStabilityTest.cpp 
											../Block.h ../Block.cpp
											../Layer.h ../Layer.cpp
											../cost.h  ../cost.cpp
											../Stability.h 	../Stability.cpp
											../HelperFunctions.h ../HelperFunctions.cpp
											)
target_link_libraries(emptyLayerStabilityTest wxgl ysclass)
add_test(NAME emptyLayerStabilityTest_build COMMAND cmake --build . --config Release --target emptyLayerStabilityTest)
add_test(NAME emptyLayerStabilityTest COMMAND emptyLayerStabilityTest)

add_executable(unstableLayerTest ${EXE_TYPE} unstableLayerTest.cpp
											../Block.h ../Block.cpp
											../Layer.h ../Layer.cpp
											../cost.h  ../cost.cpp
											../Stability.h 	../Stability.cpp
											../HelperFunctions.h ../HelperFunctions.cpp
											)

target_link_libraries(unstableLayerTest wxgl ysclass)
add_test(NAME unstableLayerTest_build COMMAND cmake --build . --config Release --target unstableLayerTest)
add_test(NAME unstableLayerTest COMMAND unstableLayerTest)

add_executable(stableLayerTest ${EXE_TYPE} stableLayerTest.cpp
											../Block.h ../Block.cpp
											../Layer.h ../Layer.cpp
											../cost.h  ../cost.cpp
											../Stability.h 	../Stability.cpp
											../HelperFunctions.h ../HelperFunctions.cpp
											)

target_link_libraries(stableLayerTest wxgl ysclass)
add_test(NAME stableLayerTest_build COMMAND cmake --build . --config Release --target stableLayerTest)
add_test(NAME stableLayerTest COMMAND stableLayerTest)

#------------------------------------------- Merge Tests -------------------------------------------#

add_executable(merge_tests ${EXE_TYPE} merge_tests.cpp 
										../Block.h 		../Block.cpp
										../cost.h 		../cost.cpp
										../Layer.h 		../Layer.cpp 
										../Merge.h 		../Merge.cpp 
										../WorkerThread.h ../WorkerThread.cpp
										../Stability.h 	../Stability.cpp
										../Voxelize.h 	../Voxelize.cpp
										../HelperFunctions.h ../HelperFunctions.cpp
										)

add_test(NAME merge_test1_build COMMAND cmake --build . --config Release --target merge_tests)
add_test(NAME merge_test1 COMMAND merge_tests Test1)
add_test(NAME merge_test2 COMMAND merge_tests Test2)
add_test(NAME merge_test3 COMMAND merge_tests Test3)


#------------------------------------------- Voxelization Tests -------------------------------------------#

add_executable(voxelize_test ${EXE_TYPE} voxelize_test.cpp 
                                        ../Block.h      ../Block.cpp
                                        
                                        ../Voxelize.h   ../Voxelize.cpp
                                        )

add_test(NAME voxelize_test_build COMMAND cmake --build . --config Release --target voxelize_test)
add_test(NAME voxelize_test1 COMMAND voxelize_test ${CMAKE_BINARY_DIR}/build/cone.stl)
add_test(NAME voxelize_test2 COMMAND voxelize_test ${CMAKE_BINARY_DIR}/build/sphere.stl)
add_test(NAME voxelize_test3 COMMAND voxelize_test ${CMAKE_BINARY_DIR}/build/plate.stl)

#------------------------------------------- Block Tests -------------------------------------------#

add_executable(BlockInitTest ${EXE_TYPE} BlockInitTest.cpp 
                                        ../Block.h      ../Block.cpp)

add_test(NAME BlockInitTestBuild COMMAND cmake --build . --config Release --target BlockInitTest)
add_test(NAME BlockInitTest COMMAND BlockInitTest)


# ------------------------------------------- Test Properties ------------------------------------------- #

set_tests_properties(
					#-------- GUI Tests --------#
					BuildTest 
					RenderSTLTestBuild 
					ExportBoMTestBuild
					ImportTestBuild
					RenderSTLTest 
					ExportBoMTest
					ImportTest

					#-------- Stability Tests --------#
					emptyLayerStabilityTest_build
					emptyLayerStabilityTest
					unstableLayerTest_build
					unstableLayerTest
					stableLayerTest_build
					stableLayerTest

					#-------- Merge Tests --------#
					merge_test1_build
					merge_test1 
					merge_test2 
					merge_test3

					#-------- Voxelize Tests--------#
					voxelize_test_build
					voxelize_test1 
					voxelize_test2 
					voxelize_test3

					# Cost Tests


					# Layer Tests


					# Block Tests
					BlockInitTestBuild
					BlockInitTest


					PROPERTIES LABELS "Blocked")