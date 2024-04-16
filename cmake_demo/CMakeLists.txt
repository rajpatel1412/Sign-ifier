# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(assignment_1 VERSION 1.0 DESCRIPTION "Signifier" LANGUAGES C)

# Compiler options (inherited by sub-folders)
set(CMAKE_C_STANDARD 11)
add_compile_options(-g -Wall -Werror -Wpedantic -Wextra)
# add_compile_options(-Wall -Wpedantic -Wextra)
add_compile_options(-lv4l2)
add_compile_options(-fdiagnostics-color)

# Enable address sanitizer
# (Comment this out to make your code faster)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# What folders to build
add_subdirectory(hal)  
add_subdirectory(app)

