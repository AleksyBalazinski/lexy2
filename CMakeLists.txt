cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(lexy2 VERSION 1.0)

### ANTLR ###
# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})
# set variable pointing to the antlr tool that supports C++
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr-4.13.0-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

### GoogleTest ###
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5197b1a8e6a1ef9f214f4aa537b0be17cbf91946.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

### Project ###
# Call macro to add lexer and grammar to your build dependencies.
antlr_target(lexy2Parser grammar/Lexy2.g4 LISTENER OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(lexy2 ${SRC_FILES} ${ANTLR_lexy2Parser_CXX_OUTPUTS})
target_include_directories(lexy2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/libs/)

# link required libraries
target_link_libraries(lexy2 antlr4_static ${SYSTEM_LIBS})

### Tests ###
file(GLOB_RECURSE LIB_SRC_FILES src/*.cpp)
list(REMOVE_ITEM LIB_SRC_FILES "src/main.cpp")
add_library(lexy2lib ${LIB_SRC_FILES} ${ANTLR_lexy2Parser_CXX_OUTPUTS})
target_include_directories(lexy2lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/libs/)

enable_testing()
file(GLOB_RECURSE TEST_FILES test/*.cpp)
add_executable(tests ${TEST_FILES})
target_link_libraries(tests GTest::gtest_main lexy2lib)

include(GoogleTest)
gtest_discover_tests(tests)