cmake_minimum_required(VERSION 3.0)

project(subsis)

set(IMGUI_DIR ./)

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARIES})

set(SOURCES
        src/app.cpp
        ${IMGUI_DIR}/imgui/imconfig.h
        src/database.cpp
        src/font.ttf
        ${IMGUI_DIR}/imgui/imgui.h
        ${IMGUI_DIR}/imgui/imgui.cpp
        ${IMGUI_DIR}/imgui/imgui_demo.cpp
        ${IMGUI_DIR}/imgui/imgui_draw.cpp
        ${IMGUI_DIR}/imgui/imgui_tables.cpp
        ${IMGUI_DIR}/imgui/imgui_widgets.cpp
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