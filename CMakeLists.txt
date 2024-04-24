cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

file(GLOB_RESCURSE COMPILE_FILES
    *.cpp
    *.hpp
)

# add_executable(Rasterizer 
#     main.cpp 
#     rasterizer.hpp rasterizer.cpp 
#     Triangle.hpp Triangle.cpp 
#     Transformation.cpp Transformation.hpp
#     Cube.cpp Cube.hpp
# )

add_executable(Rasterizer COMPILE_FILES)
target_link_libraries(Rasterizer ${OpenCV_LIBRARIES})
