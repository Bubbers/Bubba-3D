set(LIBRARY_NAME Bubba3D)

set(BUBBA3D_SOURCE_FILES "")

add_subdirectory(Input)
add_subdirectory(Misc)
add_subdirectory(cameras)
add_subdirectory(collision)
add_subdirectory(components)
add_subdirectory(core)
add_subdirectory(logging)
add_subdirectory(objects)
add_subdirectory(particle)
add_subdirectory(shader)
add_subdirectory(sound)
add_subdirectory(linmath)
add_subdirectory(glutil)
add_subdirectory(userInterface)


set(HEADER_FILES
		  ${PROJECT_SOURCE_DIR}/includes/IRenderComponent.h 
		  ${PROJECT_SOURCE_DIR}/includes/ParticleGenerator.h
		  ${PROJECT_SOURCE_DIR}/includes/AABB2.h 
		  ${PROJECT_SOURCE_DIR}/includes/IShader.h
		  ${PROJECT_SOURCE_DIR}/includes/PerspectiveCamera.h 
		  ${PROJECT_SOURCE_DIR}/includes/AudioManager.h 
		  ${PROJECT_SOURCE_DIR}/includes/Input.h 
		  ${PROJECT_SOURCE_DIR}/includes/Renderer.h 
		  ${PROJECT_SOURCE_DIR}/includes/BFBroadPhase.h 
		  ${PROJECT_SOURCE_DIR}/includes/JoystickAxis.h 
		  ${PROJECT_SOURCE_DIR}/includes/ResourceManager.h 
		  ${PROJECT_SOURCE_DIR}/includes/BroadPhase.h 
		  ${PROJECT_SOURCE_DIR}/includes/JoystickButton.h 
		  ${PROJECT_SOURCE_DIR}/includes/Scene.h 
		  ${PROJECT_SOURCE_DIR}/includes/Camera.h 
		  ${PROJECT_SOURCE_DIR}/includes/JoystickTranslation.h 
 		  ${PROJECT_SOURCE_DIR}/includes/ShaderProgram.h 
		  ${PROJECT_SOURCE_DIR}/includes/Chunk.h 
		  ${PROJECT_SOURCE_DIR}/includes/JoystickTranslator.h 
		  ${PROJECT_SOURCE_DIR}/includes/SkyBoxRenderer.h 
		  ${PROJECT_SOURCE_DIR}/includes/Collider.h 
		  ${PROJECT_SOURCE_DIR}/includes/KeyboardButton.h 
		  ${PROJECT_SOURCE_DIR}/includes/Sphere.h 
		  ${PROJECT_SOURCE_DIR}/includes/ControlStatus.h 
		  ${PROJECT_SOURCE_DIR}/includes/Lights.h 
		  ${PROJECT_SOURCE_DIR}/includes/StandardRenderer.h 
		  ${PROJECT_SOURCE_DIR}/includes/Controls.h 
		  ${PROJECT_SOURCE_DIR}/includes/LogHandler.h 
		  ${PROJECT_SOURCE_DIR}/includes/StdOutLogHandler.h 
		  ${PROJECT_SOURCE_DIR}/includes/ControlsManager.h 
		  ${PROJECT_SOURCE_DIR}/includes/Logger.h 
		  ${PROJECT_SOURCE_DIR}/includes/Texture.h 
		  ${PROJECT_SOURCE_DIR}/includes/CubeMapTexture.h 
		  ${PROJECT_SOURCE_DIR}/includes/Material.h 
		  ${PROJECT_SOURCE_DIR}/includes/Triangle.h
		  ${PROJECT_SOURCE_DIR}/includes/DestructorUtils.h 
		  ${PROJECT_SOURCE_DIR}/includes/Mesh.h 
		  ${PROJECT_SOURCE_DIR}/includes/Utils.h 
		  ${PROJECT_SOURCE_DIR}/includes/Effects.h 
		  ${PROJECT_SOURCE_DIR}/includes/MouseAxis.h 
		  ${PROJECT_SOURCE_DIR}/includes/Window.h 
		  ${PROJECT_SOURCE_DIR}/includes/FileLogHandler.h 
		  ${PROJECT_SOURCE_DIR}/includes/MouseButton.h 
		  ${PROJECT_SOURCE_DIR}/includes/constants.h 
		  ${PROJECT_SOURCE_DIR}/includes/GameObject.h 
		  ${PROJECT_SOURCE_DIR}/includes/MousePosition.h 
		  ${PROJECT_SOURCE_DIR}/includes/glutil/glutil.h 
		  ${PROJECT_SOURCE_DIR}/includes/Globals.h 
		  ${PROJECT_SOURCE_DIR}/includes/MouseWarp.h 
		  ${PROJECT_SOURCE_DIR}/includes/linmath 
		  ${PROJECT_SOURCE_DIR}/includes/HudRenderer.h 
		  ${PROJECT_SOURCE_DIR}/includes/MoveComponent.h 
		  ${PROJECT_SOURCE_DIR}/includes/rapidjson 	 
		  ${PROJECT_SOURCE_DIR}/includes/IComponent.h 
		  ${PROJECT_SOURCE_DIR}/includes/Octree.h 
		  ${PROJECT_SOURCE_DIR}/includes/stb_image.h 
		  ${PROJECT_SOURCE_DIR}/includes/IDrawable.h 
		  ${PROJECT_SOURCE_DIR}/includes/timer.h
 		  ${PROJECT_SOURCE_DIR}/includes/IJoystickTranslation.h
 		  ${PROJECT_SOURCE_DIR}/includes/ParticleConf.h)

add_library(${LIBRARY_NAME} STATIC ${BUBBA3D_FILES_SOURCE} ${HEADER_FILES})

#########################################################
# STRUCTURE MSVS
#########################################################

source_group(Cameras	REGULAR_EXPRESSION cameras/ )
source_group(Collision  REGULAR_EXPRESSION collision/ )
source_group(Components REGULAR_EXPRESSION components/ ) 
source_group(Core       REGULAR_EXPRESSION core/ )
source_group(Glutil     REGULAR_EXPRESSION glutil/ )
source_group(Input      REGULAR_EXPRESSION Input/)
source_group(Linmath    REGULAR_EXPRESSION linmath/)
source_group(Logging	REGULAR_EXPRESSION logging/)
source_group(Misc	REGULAR_EXPRESSION Misc/)
source_group(Objects	REGULAR_EXPRESSION objects/)
source_group(Particle   REGULAR_EXPRESSION particle/)
source_group(Shader     REGULAR_EXPRESSION shader/)
source_group(Sound      REGULAR_EXPRESSION sound/)



 




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


if(NOT SFML_FOUND)
    message(ERROR " SFML not found!")
endif(NOT SFML_FOUND)

#########################################################
# FIND FREETYPE
#########################################################
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

target_include_directories (${LIBRARY_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/includes")
target_include_directories (${LIBRARY_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib")
target_include_directories (${LIBRARY_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/lib/freeimage")

target_link_libraries(${LIBRARY_NAME}  ${SFML_LIBRARIES})


target_link_libraries(${LIBRARY_NAME}  ${ASSIMP_LIBRARY})
target_link_libraries(${LIBRARY_NAME}  ${ASSIMP_LIBRARIES})

target_link_libraries(${LIBRARY_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES} ${ASSIMP_LIBRARY} ${SFML_LIBRARIES})

target_link_libraries(${LIBRARY_NAME} ${FREETYPE_LIBRARIES})
