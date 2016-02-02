cmake_minimum_required(VERSION 3.0)

set(COLLIDER_TEST_NAME Bubba3DTestCollider)
set(OCTREE_TEST_NAME   Bubba3DTestOctree)
set(HUD_RENDERER_TEST  Bubba3DTestHud)

add_executable(${COLLIDER_TEST_NAME} collider_test.cpp)
add_test(NAME TestSuiteCollider COMMAND ${COLLIDER_TEST_NAME})
target_include_directories (${COLLIDER_TEST_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(${COLLIDER_TEST_NAME} LINK_PUBLIC Bubba3D)

add_executable(${OCTREE_TEST_NAME}   octree_test.cpp)
add_test(NAME TestSuiteOctree   COMMAND ${OCTREE_TEST_NAME})
target_include_directories (${OCTREE_TEST_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(${OCTREE_TEST_NAME} LINK_PUBLIC Bubba3D)

add_executable(${HUD_RENDERER_TEST}  hud_renderer_test.cpp)
add_test(NAME TestSuiteHud   COMMAND ${HUD_RENDERER_TEST})
target_include_directories (${HUD_RENDERER_TEST} PUBLIC "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(${HUD_RENDERER_TEST} LINK_PUBLIC Bubba3D)

# configure unit tests via CTest
enable_testing()







