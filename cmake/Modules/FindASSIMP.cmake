# - Try to find Assimp
# Once done, this will define
#
#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIR - the Assimp include directories
#  ASSIMP_LIBRARIES - link these to use Assimp

#FIND_PATH( ASSIMP_INCLUDE_DIR assimp/mesh.h
#        /usr/include
#        /usr/local/include
#        /opt/local/include
#        /sw/include
#        )

FIND_LIBRARY( ASSIMP_LIBRARY assimp libassimp
        /usr/lib64
        /usr/lib
        /usr/lib/x86_64-linux-gnu
        /usr/local/lib
        /opt/local/lib
        /usr/local/lib64
        /usr/local/lib
        /sw/lib
        )


FIND_PATH(ASSIMP_INCLUDE_DIR assimp/ai_assert.h
        $ENV{ASSIMPSDIR}/include
        $ENV{ASSIMPSDIR}
        $ENV{ASSIMPSDIR}/..
        ~/Library/Frameworks/AssImp.framework/Headers
        /Library/Frameworks/AssImp.framework/Headers
        /usr/local/include/assimp
        /usr/local/include
        /usr/include/assimp
        /home/travis
        /home/travis/build/Bubbers/Bubba-3D/lib
        /usr/include
        /sw/include/assimp # Fink
        /sw/include
        /opt/local/include/assimp # DarwinPorts
        /opt/local/include
        /opt/csw/include/assimp # Blastwave
        /opt/csw/include
        /opt/include/assimp
        /opt/include
        ${_assimp_INCLUDE_SEARCH_DIRS_SYSTEM}
        )

IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
    SET( ASSIMP_FOUND TRUE )
    SET( ASSIMP_LIBRARIES ${ASSIMP_LIBRARY} )
ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)

IF(ASSIMP_FOUND)
    IF(NOT ASSIMP_FIND_QUIETLY)
        MESSAGE(STATUS "Found ASSIMP: ${ASSIMP_LIBRARY}")
    ENDIF(NOT ASSIMP_FIND_QUIETLY)
ELSE(ASSIMP_FOUND)
    IF(ASSIMP_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find libASSIMP")
    ENDIF(ASSIMP_FIND_REQUIRED)
ENDIF(ASSIMP_FOUND)