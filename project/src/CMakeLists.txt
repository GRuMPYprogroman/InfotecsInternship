set(LOGGER_SOURCES 
        FileLogger.cpp
        SocketLogger.cpp)

option(BUILD_SHARED "Build Logger as a shared library" ON)

if (BUILD_SHARED)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif()

add_library(logger ${LIB_TYPE} ${LOGGER_SOURCES})

target_include_directories(logger PUBLIC 
        ../include/AppLogEvents
        ../include/Logger
        ../include/Utilities
)