# Create the library target.
# We start with just one source file.
add_library(dsa_core src/vector.cpp)

# Define the "Public" interface.
# Any target that links to 'dsa_core' will automatically
# get 'include/' added to their search path.
target_include_directories(dsa_core PUBLIC include)

# Enable standard compiler warnings
if (MSVC)
    target_compile_options(dsa_core PRIVATE /W4)
else()
    target_compile_options(dsa_core PRIVATE -Wall -Wextra -pedantic)
endif()

# Enable Architecture-Specific Optimizations for Release Builds
# -march=native allows the compiler to use instructions specific
# to your CPU (like AVX)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(dsa_core PRIVATE $<$<CONFIG:Release>:-O3 -march=native>)
elseif(MSVC)
    target_compile_options(dsa_core PRIVATE $<$<CONFIG:Release>:/O2 /arch:AVX2>)
endif()

# Enable Link Time Optimization (LTO) if supported.
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)

if (result)
    set_target_properties(dsa_core PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "IPO is not supported: ${output}")
endif ()