# CMake generated Testfile for 
# Source directory: C:/Users/badma/CLionProjects/sfml-try
# Build directory: C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Tetramino-tests "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/Tetramino-tests.exe" "--force-color" "-d")
set_tests_properties(Tetramino-tests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/badma/CLionProjects/sfml-try/CMakeLists.txt;21;add_test;C:/Users/badma/CLionProjects/sfml-try/CMakeLists.txt;0;")
subdirs("_deps/sfml-build")
subdirs("doctest")
