cmake_minimum_required(VERSION 3.0)

project(traingame)

set(TRAINGAME_SRC
    main.cpp
    GaugeDataStore.cpp
    RailProfileDataStore.cpp

find_package(nlohmann_json REQUIRED)

add_executable(traingame ${TRAINGAME_SRC})

install(TARGETS traingame RUNTIME DESTINATION bin)
