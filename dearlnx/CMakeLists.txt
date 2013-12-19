cmake_minimum_required(VERSION 2.8)
project (DEARLNX)

# cmake
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${DEARLNX_SOURCE_DIR}/cmake")

# Collect shit
file (GLOB SRC_CPP ${DEARLNX_SOURCE_DIR}/*.cpp)

# Declare application
add_executable(dearlnx ${SRC_CPP})

# find shit
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED libglfw)
find_package(SOIL REQUIRED)

#include shit
include_directories( ${OPENGL_INCLUDE_DIRS} )
include_directories( ${GLFW_INCLUDE_DIRS} )
include_directories( ${GLEW_INCLUDE_DIRS} )
include_directories( ${SOIL_INCLUDE_DIRS} )
include_directories( /opt/libjpeg-turbo/include )

#link shit
target_link_libraries(dearlnx ${OPENGL_LIBRARIES} )
target_link_libraries(dearlnx ${GLFW_LIBRARIES} )
target_link_libraries(dearlnx ${GLEW_LIBRARIES} )
target_link_libraries(dearlnx ${SOIL_LIBRARIES} )

# Copy resources to build directory
file(COPY ${DEARLNX_SOURCE_DIR}/resources DESTINATION ${DEARLNX_BINARY_DIR} )


