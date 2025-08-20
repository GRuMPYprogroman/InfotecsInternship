add_executable(testApp TestApp.cpp)
target_compile_features(testApp PRIVATE cxx_std_17)

target_link_libraries(testApp PRIVATE logger)