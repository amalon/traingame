cmake_minimum_required(VERSION 3.0)

project(traingame)

set(TRAINGAME_SRC
    main.cpp
    GaugeDataStore.cpp
    RailProfileDataStore.cpp
    Window.cpp
    WindowSDL.cpp
    Renderable.cpp
    Renderer.cpp
    RendererOpenGL.cpp
    Viewpoint.cpp
    ControlMode.cpp
    TrackMode.cpp
    NavigateMode.cpp
    Railway.cpp
    TrackNode.cpp
    TrackSection.cpp
    RailwayGL.cpp
    TrackModeGL.cpp
    TrackSectionGL.cpp)

find_package(nlohmann_json REQUIRED)
find_package(SDL2 REQUIRED)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

add_executable(traingame ${TRAINGAME_SRC})
target_link_libraries(traingame PUBLIC SDL2::SDL2 OpenGL::OpenGL OpenGL::EGL)
target_include_directories(traingame PRIVATE ".")

install(TARGETS traingame RUNTIME DESTINATION bin)
