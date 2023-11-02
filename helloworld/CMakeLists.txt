cmake_minimum_required(VERSION 3.0)  # Ensure a minimum CMake version of 3.0

project("Hello world")  # Define the project with the name "Hello world"
# Replace "Hello world" with your project name.

# Include the path to the JavaScriptCore header files
include_directories("/usr/include/webkitgtk-4.0")

# Add the executable
add_executable(helloworld helloworld.c)
# Create an executable named "helloworld" using the source file "helloworld.c".
# Replace "helloworld.c" with your actual source code file.

# Link with JavaScriptCore
target_link_libraries(helloworld javascriptcoregtk-4.0)
# Link the "helloworld" executable with the "javascriptcoregtk-4.0" library.
