add_executable(Client Client.cpp)
add_executable(Server Server.cpp)

target_compile_features(Client PRIVATE cxx_std_17)
target_compile_features(Server PRIVATE cxx_std_17)

target_link_libraries(Client PRIVATE logger)
target_include_directories(Server PRIVATE 
        ../include/Utilities
)