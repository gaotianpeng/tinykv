cmake_minimum_required(VERSION 3.9)
project(tinykv VERSION 0.0 LANGUAGES C CXX)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 11)
  set(CMAKE_C_STANDARD_REQUIRED OFF)
  set(CMAKE_C_EXTENSIONS OFF)
endif(NOT CMAKE_C_STANDARD)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif(NOT CMAKE_CXX_STANDARD)

option(TINYKV_BUILD_TESTS "Build tinykv's unit tests" ON)
option(TINYKV_BUILD_BENCHMARKS "Build tinykv's benchmarks" ON)
option(TINYKV_INSTALL "Install tinykv's header and library" ON)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)

include(CheckLibraryExists)
check_library_exists(crc32c crc32c_value "" HAVE_CRC32C)
check_library_exists(snappy snappy_compress "" HAVE_SNAPPY)
check_library_exists(tcmalloc malloc "" HAVE_TCMALLOC)

include(CheckCXXSymbolExists)
check_cxx_symbol_exists(fdatasync "unistd.h" HAVE_FDATASYNC)
check_cxx_symbol_exists(F_FULLFSYNC "fcntl.h" HAVE_FULLFSYNC)
check_cxx_symbol_exists(O_CLOEXEC "fcntl.h" HAVE_O_CLOEXEC)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Disable C++ exceptions.
  string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs-c-")
  add_definitions(-D_HAS_EXCEPTIONS=0)

  # Disable RTTI.
  string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Enable strict prototype warnings for C code in clang and gcc.
  if(NOT CMAKE_C_FLAGS MATCHES "-Wstrict-prototypes")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-prototypes")
  endif(NOT CMAKE_C_FLAGS MATCHES "-Wstrict-prototypes")

  # Disable C++ exceptions.
  string(REGEX REPLACE "-fexceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

  # Disable RTTI.
  string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wthread-safety HAVE_CLANG_THREAD_SAFETY)

# Used by googletest.
check_cxx_compiler_flag(-Wno-missing-field-initializers
                        TINYKV_HAVE_NO_MISSING_FIELD_INITIALIZERS)

if(BUILD_SHARED_LIBS)
    # Only export LEVELDB_EXPORT symbols from the shared library.
    add_compile_options(-fvisibility=hidden)
endif(BUILD_SHARED_LIBS)

# GoogleTest triggers a missing field initializers warning.
if(TINYKV_HAVE_NO_MISSING_FIELD_INITIALIZERS)
  set_property(TARGET gtest
      APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
  set_property(TARGET gmock
      APPEND PROPERTY COMPILE_OPTIONS -Wno-missing-field-initializers)
endif(TINYKV_HAVE_NO_MISSING_FIELD_INITIALIZERS)

include(GNUInstallDirs)

add_executable(tinykv "")
target_sources(tinykv
PUBLIC
  "db/db.cc"
)

if(HAVE_CRC32C)
  target_link_libraries(tinykv crc32c)
endif(HAVE_CRC32C)
if(HAVE_SNAPPY)
  target_link_libraries(tinykv snappy)
endif(HAVE_SNAPPY)
if(HAVE_TCMALLOC)
  target_link_libraries(tinykv tcmalloc)
endif(HAVE_TCMALLOC)

find_package(Threads REQUIRED)
target_link_libraries(tinykv Threads::Threads)


enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(install_gtest OFF)
set(install_gmock OFF)
set(build_gmock ON)
add_subdirectory("third_party/googletest")

add_executable(tinykv_tests "")
target_sources(tinykv_tests
  PUBLIC
  "db/db_test.cc"
)                      
target_link_libraries(tinykv_tests gmock gtest gtest_main)