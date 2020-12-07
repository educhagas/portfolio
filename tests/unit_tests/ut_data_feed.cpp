#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <portfolio/data_feed/mock_data_feed.h>

TEST_CASE("Data Feed") {
    using namespace portfolio;
    mock_data_feed m;
    data_feed_result r = m.fetch("PETR3");
    REQUIRE(r.current_price() != 0.0);
}
