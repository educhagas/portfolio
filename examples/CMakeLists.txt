find_package(Matplot++ QUIET)

if (Matplot++_FOUND)
    add_executable(example_data_feed data_feed.cpp)
    target_link_libraries(example_data_feed PRIVATE portfolio Matplot++::matplot)
    target_exception_options(example_data_feed)
endif ()

add_executable(example_evolutionary_algorithm_nsga2 evolutionary_algorithm_nsga2.cpp)
target_link_libraries(example_evolutionary_algorithm_nsga2 PRIVATE portfolio Matplot++::matplot pareto)
target_exception_options(example_evolutionary_algorithm_nsga2)

add_executable(example_previous_portfolio example_previous_portfolio.cpp)
target_link_libraries(example_previous_portfolio PRIVATE portfolio pareto)
target_exception_options(example_previous_portfolio)

add_executable(example_cost_portfolio example_cost_portfolio.cpp)
target_link_libraries(example_cost_portfolio PRIVATE portfolio pareto)
target_exception_options(example_cost_portfolio)