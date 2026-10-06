# Create the executable binary
add_executable(dsa_app main.cpp
        PlayerStorageUse.cpp)

# Link to our core library.
# This pulls in the code AND the include paths
target_link_libraries(dsa_app PRIVATE dsa_core)