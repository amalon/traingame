cmake_minimum_required(VERSION 3.0)

project(traingame)

set(TRAINGAME_SRC
    main.cpp
    GaugeDataStore.cpp
    RailProfileDataStore.cpp
    Window.cpp
    WindowSDL.cpp
    RendererOpenGL.cpp
    Viewpoint.cpp)

find_package(nlohmann_json REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(traingame ${TRAINGAME_SRC})
target_link_libraries(traingame PUBLIC SDL2::SDL2 OpenGL::GL)

install(TARGETS traingame RUNTIME DESTINATION bin)
