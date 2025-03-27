add_executable(compute_pipeline
    example.cpp
    ComputePipeline.cpp
    ComputePipeline.h
    ComputeOperations.cpp
    ComputeOperations.h
)

target_include_directories(compute_pipeline PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(compute_pipeline PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(compute_pipeline PRIVATE /W4)
else()
    target_compile_options(compute_pipeline PRIVATE -Wall -Wextra)
endif()
