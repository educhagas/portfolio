#######################################################
### Dependencies                                    ###
#######################################################
# Threads
find_package(Threads)

# Ranges library
CPMAddPackage(NAME range-v3 URL https://github.com/ericniebler/range-v3/archive/0.11.0.zip VERSION 0.11.0 DOWNLOAD_ONLY TRUE)
add_library(range-v3 INTERFACE IMPORTED)
target_include_directories(range-v3 INTERFACE "${range-v3_SOURCE_DIR}/include")

# Howard Hinnant's date library
CPMAddPackage(
        NAME date
        GITHUB_REPOSITORY HowardHinnant/date
        VERSION 3.0.0
)

#######################################################
### Library                                         ###
#######################################################
add_library(portfolio
        portfolio/data_feed/data_feed_result.cpp
        portfolio/data_feed/data_feed_result.h
        portfolio/data_feed/data_feed.cpp
        portfolio/data_feed/data_feed.h
        portfolio/data_feed/mock_data_feed.cpp
        portfolio/data_feed/mock_data_feed.h
        portfolio/market_data.cpp
        portfolio/market_data.h
        portfolio/portfolio.cpp
        portfolio/portfolio.h
)
target_include_directories(portfolio
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(portfolio PUBLIC Threads::Threads range-v3 date::date)
target_pedantic_options(portfolio)
target_exception_options(portfolio)