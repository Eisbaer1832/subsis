cmake_minimum_required(VERSION 3.0)

project(subsis)

set(IMGUI_DIR ./)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/modules")

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(src/data/)
link_libraries(${GLFW_LIBRARIES})

set(SOURCES
        src/data/maps/test.sub
        src/app.cpp
        ${IMGUI_DIR}/backends/imconfig.h
        src/database.cpp
        src/font.ttf
        ${IMGUI_DIR}/backends/imgui.h
        ${IMGUI_DIR}/backends/imgui.cpp
        ${IMGUI_DIR}/backends/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_draw.cpp
        ${IMGUI_DIR}/backends/imgui_tables.cpp
        ${IMGUI_DIR}/backends/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        )

set(CMAKE_CXX_STANDARD 11)

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# add_definitions(-DIMGUI_IMPL_OPENGL_ES2)
# set(LINUX_GL_LIBS -lGLESv2)

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

if(UNIX AND NOT APPLE)
    set(ECHO_MESSAGE "Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)

    set(LIBS ${GLFW_LIBRARIES} ${LINUX_GL_LIBS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GLFW_CFLAGS} -g -Wall -Wformat")
endif()

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------
add_executable(subsis ${SOURCES})
target_include_directories(subsis PRIVATE ${IMGUI_DIR} glfw)
target_link_libraries(subsis ${LIBS} -lGL -lGLU -lGLEW -lglfw)