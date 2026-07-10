cmake_minimum_required(VERSION 3.28)
project(BlursedECS LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/walterschell/Lua.git
    GIT_TAG v5.4.7
    SYSTEM
    EXCLUDE_FROM_ALL
)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.5.0
    SYSTEM
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(lua sol2)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(BlursedECS ${SRC})
target_compile_features(BlursedECS PRIVATE cxx_std_20)
target_link_libraries(BlursedECS PRIVATE lua_static sol2)
