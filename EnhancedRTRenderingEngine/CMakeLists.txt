cmake_minimum_required(VERSION, 2.8)
project(EnhancedRTRenderingEngine CXX)

MACRO(subdirlist result curdir)
  FILE(GLOB_RECURSE children LIST_DIRECTORIES true ${curdir}/*)
  SET(dirlist)
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SET(CMAKE_CXX_FLAGS "-std=c++14 -stdlib=libc++")

set(SOURCES)
set(LOCAL_INCLUDE_PATH)

if(APPLE)
FIND_PACKAGE(OpenGL REQUIRED)
MESSAGE(STATUS ${OPENGL_LIBRARIES})
MESSAGE(STATUS ${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(GLUT REQUIRED)
MESSAGE(STATUS ${GLUT_LIBRARY})
MESSAGE(STATUS ${GLUT_INCLUDE_DIR})

INCLUDE_DIRECTORIES(
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    )

target_link_libraries(EnhancedRTRenderingEngine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
elseif(WIN32)
endif()

file(GLOB_RECURSE auto_append_sources *.cpp)
list(APPEND SOURCES ${auto_append_sources})
file(GLOB_RECURSE auto_append_c *.c)
list(APPEND SOURCES ${auto_append_c})
add_executable(EnhancedRTRenderingEngine SOURCES)

list(APPEND LOCAL_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND LOCAL_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ThirdPirty/include)

target_link_libraries(libping16 zlib)



