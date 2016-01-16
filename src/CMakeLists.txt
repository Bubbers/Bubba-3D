set(LIBRARY_NAME Bubba3D)


add_library(${LIBRARY_NAME} SHARED ${BUBBA3D_FILES_SOURCE})

#########################################################
# STRUCTURE MSVS
#########################################################
source_group(Logging	FILES logging/Logger.cpp)
source_group(Cameras	FILES cameras/Camera.cpp cameras/PerspectiveCamera.cpp)
source_group(Objects	FILES objects/Mesh.cpp objects/Scene.cpp objects/SkyBoxRenderer.cpp objects/Triangle.cpp)
source_group(Collision	FILES collision/Collider.cpp collision/Octree.cpp)
source_group(Core		FILES core/Renderer.cpp)
source_group(Misc		FILES Misc/CubeMapTexture.cpp ${PROJECT_SOURCE_DIR}/includes/glutil/glutil.cpp)
source_group(Particle   FILES particle/Particle.cpp particle/FireParticle.cpp particle/ParticleGenerator.cpp)
source_group(Math		FILES
        ${PROJECT_SOURCE_DIR}/includes/linmath/float2.cpp
        ${PROJECT_SOURCE_DIR}/includes/linmath/float3.cpp
        ${PROJECT_SOURCE_DIR}includes/linmath/float3x3.cpp
        ${PROJECT_SOURCE_DIR}/includes/linmath/float4.cpp
        ${PROJECT_SOURCE_DIR}/includes/linmath/float4x4.cpp
        ${PROJECT_SOURCE_DIR}/includes/linmath/int2.cpp
        ${PROJECT_SOURCE_DIR}/includes/linmath/int3.cpp
        ${PROJECT_SOURCE_DIR}/includes/linmath/int4.cpp
        ${PROJECT_SOURCE_DIR}/includes/linmath/Quaternion.cpp)
source_group(Shaders    FILES
        ${PROJECT_SOURCE_DIR}/shaders/simple.frag
        ${PROJECT_SOURCE_DIR}/shaders/simple.vert
        ${PROJECT_SOURCE_DIR}/shaders/skybox.frag
        ${PROJECT_SOURCE_DIR}/shaders/skybox.vert
        ${PROJECT_SOURCE_DIR}/shaders/vertical_blur.frag
        ${PROJECT_SOURCE_DIR}/shaders/horizontal_blur.frag
        ${PROJECT_SOURCE_DIR}/shaders/cutoff.frag
        ${PROJECT_SOURCE_DIR}/shaders/postFx.frag
        ${PROJECT_SOURCE_DIR}/shaders/postFx.vert
        ${PROJECT_SOURCE_DIR}/shaders/shadowMap.frag
        ${PROJECT_SOURCE_DIR}/shaders/shadowMap.vert)


#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)


#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
link_directories(${GLEW_LIBRARY})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)


#########################################################
# FIND FREEIMAGE
#########################################################
find_package(FREEIMAGE REQUIRED)
include_directories(${FREEIMAGE_INCLUDE_PATH})

if(NOT FREEIMAGE_FOUND)
    message(ERROR " FreeImage not found!")
endif(NOT FREEIMAGE_FOUND)

#########################################################
# FIND ASSIMP
#########################################################
find_package(ASSIMP REQUIRED)
if(ASSIMP_INCLUDE_DIR)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif()
add_definitions(${ASSIMP_DEFINITIONS})


if(NOT ASSIMP_FOUND)
    message(ERROR " ASSIMP not found!")
endif(NOT ASSIMP_FOUND)


#########################################################
# FIND SFML AUDIO
#########################################################
find_package(SFML 2 COMPONENTS audio window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME}  ${SFML_LIBRARIES})

if(NOT SFML_FOUND)
    message(ERROR " SFML not found!")
endif(NOT SFML_FOUND)


target_include_directories (${LIBRARY_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes")
target_include_directories (${LIBRARY_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes/linmath")
target_include_directories (${LIBRARY_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes/rapidjson")

target_link_libraries(${LIBRARY_NAME}  ${FREEIMAGE_LIBRARY})
target_link_libraries(${LIBRARY_NAME}  ${FREEIMAGE_LIBRARIES})

target_link_libraries(${LIBRARY_NAME}  ${ASSIMP_LIBRARY})
target_link_libraries(${LIBRARY_NAME}  ${ASSIMP_LIBRARIES})

target_link_libraries(${LIBRARY_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} ${ASSIMP_LIBRARY} ${SFML_LIBRARIES})
