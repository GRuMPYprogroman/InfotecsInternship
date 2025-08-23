cmake_minimum_required(VERSION 3.28)
project(LoggerLibrary 
        VERSION 1.0 
        LANGUAGES CXX 
        DESCRIPTION "Internship task for Infotecs")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(TestApp)
add_subdirectory(TestSocketApp)

# Testing only available if this is the main app
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME))
    add_subdirectory(tests)
    enable_testing()
endif()

