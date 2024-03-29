#######################################################
### Catch2 - Unit tests                             ###
#######################################################
# Find or get Catch2
# Note that CPM will try to "find_package(Catch2)" before downloading it
# See the option CPM_USE_LOCAL_PACKAGES in ROOT/CMakeLists.txt
# This is important: see One Definition Rule (ODR)
CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 2.12.1
        OPTIONS
        "CATCH_USE_VALGRIND OFF"
        "CATCH_BUILD_TESTING OFF"
        "CATCH_BUILD_EXAMPLES OFF"
        "CATCH_BUILD_EXTRA_TESTS OFF"
        "CATCH_ENABLE_COVERAGE OFF"
        "CATCH_ENABLE_WERROR ON"
        "CATCH_INSTALL_DOCS OFF"
        "CATCH_INSTALL_HELPERS OFF"
)
include(${Catch2_SOURCE_DIR}/contrib/Catch.cmake)

#######################################################
### Tests                                           ###
#######################################################
add_executable(ut_data_feed ut_data_feed.cpp)
add_executable(ut_portfolio ut_portfolio.cpp)
add_executable(ut_portfolio_json ut_portfolio_json.cpp)

target_link_libraries(ut_data_feed PUBLIC portfolio Catch2)
target_link_libraries(ut_portfolio PUBLIC portfolio Catch2)
target_link_libraries(ut_portfolio_json PUBLIC portfolio Catch2)

catch_discover_tests(ut_data_feed ut_portfolio ut_portfolio_json)