cmake_minimum_required(VERSION 3.28)
project(ECS-TestAssignment LANGUAGES CXX)

include (FetchContent)

FetchContent_Declare(tiny-optional
    GIT_REPOSITORY https://github.com/Sedeniono/tiny-optional.git
    GIT_TAG v1.5.3
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(tiny-optional)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(ECS-TestAssignment ${SRC})
target_compile_features(ECS-TestAssignment PRIVATE cxx_std_20)
target_link_libraries(ECS-TestAssignment PRIVATE
    tiny-optional::tiny-optional
)
