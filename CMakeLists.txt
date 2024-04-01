cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(lexy2 VERSION 1.0)

### ANTLR set up ###
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


### Project ###
# Call macro to add lexer and grammar to your build dependencies.
antlr_target(lexy2Parser grammar/Lexy2.g4 LISTENER OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_executable(lexy2 ${SRC_FILES} ${ANTLR_lexy2Parser_CXX_OUTPUTS})
target_include_directories(lexy2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/libs/)

# link required libraries
target_link_libraries(lexy2 antlr4_static ${SYSTEM_LIBS})