cmake_minimum_required(VERSION 3.0)

project(Tetris)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Add main.cpp
add_executable(sfml-try main.cpp GLOBAL.hpp Tetramino.cpp Tetramino.hpp)

# Add tests_main.cpp
add_executable(tests Tetramino-tests.cpp)

# Link SFML libraries (adjust the paths as necessary)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)
target_link_libraries(sfml-try sfml-graphics sfml-window sfml-system)
target_link_libraries(tests sfml-graphics sfml-window sfml-system)

# Include Doctest
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest
        GIT_TAG v2.4.9
)
FetchContent_MakeAvailable(doctest)
target_link_libraries(tests doctest::doctest)