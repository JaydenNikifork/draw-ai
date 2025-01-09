cmake_minimum_required(VERSION 3.10)
project(DrawAI LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add the image library (header-only)
add_library(image_library INTERFACE)
target_include_directories(image_library INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../image
    ${OpenCV_INCLUDE_DIRS} # Add OpenCV's include directories
)

# Add the main executable
add_executable(main1 main.cc)

# Link OpenCV and the image library to the main executable
target_link_libraries(main1 PRIVATE image_library ${OpenCV_LIBS})
