find_package(Matplot++ QUIET)

if (Matplot++_FOUND)
    add_executable(example_data_feed data_feed.cpp)
    target_link_libraries(example_data_feed PRIVATE portfolio Matplot++::matplot)
    target_exception_options(example_data_feed)
endif ()

add_executable(example_evolutionary_algorithm evolutionary_algorithm.cpp)
target_link_libraries(example_evolutionary_algorithm PRIVATE portfolio Matplot++::matplot pareto)
target_exception_options(example_evolutionary_algorithm)