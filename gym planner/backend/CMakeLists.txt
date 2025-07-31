cmake_minimum_required(VERSION 3.5)
project(smart_workout_planner CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# Include directories
include_directories(src)
include_directories(include)

# Collect all source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.cc"
    "src/*.c"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link system libraries (for socket programming)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 wsock32)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
endif()

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()