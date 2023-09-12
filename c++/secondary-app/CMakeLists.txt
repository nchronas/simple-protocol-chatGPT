cmake_minimum_required(VERSION 3.10)

project(sec-node-app)

# Set C++ standard to C++11 or higher if needed
set(CMAKE_CXX_STANDARD 11)

# Add your source files here
set(SOURCES
    secondary-node.cpp
    ../message.cpp  # If you have a separate .cpp file for MyClass
    ../node.cpp  # If you have a separate .cpp file for MyClass
)

# Create an executable
add_executable(secondary-node ${SOURCES})

# Include the necessary headers (if any)
target_include_directories(secondary-node PRIVATE .)

# Link any additional libraries if needed
# target_link_libraries(your_program_name PRIVATE your_library_name)

# Optionally set compiler flags
# target_compile_options(your_program_name PRIVATE -Wall -Wextra)

# Specify the output binary name
# set_target_properties(your_program_name PROPERTIES OUTPUT_NAME your_program_name)
