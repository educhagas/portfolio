#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <portfolio/data_feed/mock_data_feed.h>

TEST_CASE("Data Feed") {
    using namespace portfolio;
    using namespace date::literals;
    mock_data_feed m;
    data_feed_result r = m.fetch("PETR3");
    data_feed_result r2 =
        m.fetch_from("PETR4", 2019_y / 01 / 01, 2019_y / 01 / 10);
    REQUIRE(r.current_price() != 0.0);
    REQUIRE(r2.price_from_date(2018_y / 12 / 31) ==
            r2.price_from_date(2019_y / 01 / 01));
    REQUIRE(r2.price_from_date(2019_y / 01 / 01) != 0.0);
    REQUIRE(r2.price_from_date(2020_y / 01 / 01) != 0.0);
    REQUIRE(r2.price_from_date(2020_y / 01 / 01) ==
            r2.price_from_date(2019_y / 12 / 31));
}
