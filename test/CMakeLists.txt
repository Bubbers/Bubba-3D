cmake_minimum_required(VERSION 3.0)
set(TEST_NAME Bubba3DTest)

if(USE_CPP11)
    ## We can't turn this on by default, since it breaks on travis
    message(STATUS "Enabling C++11")
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

# configure the executable

add_executable(${TEST_NAME} test.cpp ${BUBBA3D_FILES_SOURCE})


message("HAHAHHAHAHAHAHAHAHAHHAHAHAHHAHHAAHA")
message("${PROJECT_SOURCE_DIR}includes")

target_include_directories (${TEST_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes")
target_include_directories (${TEST_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes/linmath")

# configure unit tests via CTest
enable_testing()
add_test(NAME trolo COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME}  ${SFML_LIBRARIES})
target_link_libraries(${TEST_NAME}  ${FREEIMAGE_LIBRARY})
target_link_libraries(${TEST_NAME}  ${FREEIMAGE_LIBRARIES})
target_link_libraries(${TEST_NAME}  ${ASSIMP_LIBRARY})
target_link_libraries(${TEST_NAME}  ${ASSIMP_LIBRARIES})

target_link_libraries(${TEST_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} ${ASSIMP_LIBRARY})

