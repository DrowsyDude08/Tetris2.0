# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-src"
  "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-build"
  "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix"
  "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/tmp"
  "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp"
  "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src"
  "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/badma/CLionProjects/sfml-try/cmake-build-debug/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
