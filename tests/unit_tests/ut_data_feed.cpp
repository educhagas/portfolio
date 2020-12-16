#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <portfolio/data_feed/mock_data_feed.h>

TEST_CASE("Data Feed") {
    using namespace portfolio;
    using namespace date::literals;
    mock_data_feed m;
    // data_feed_result r = m.fetch("PETR3");
    data_feed_result r =
        m.fetch_from("PETR4", 2019_y / 01 / 01, 2019_y / 12 / 31);

    // If request the price of a date before the oldest date returns the price
    // of the oldest period.
    REQUIRE(r.price_from_date(2018_y / 12 / 31) ==
            r.price_from_date(2019_y / 01 / 01));
    REQUIRE(r.price_from_date(2018_y / 12 / 30) ==
            r.price_from_date(2018_y / 12 / 29));

    // If request the price of a date after the newest date returns the price
    // of the newest period.
    REQUIRE(r.price_from_date(2019_y / 12 / 31) ==
            r.price_from_date(2020_y / 01 / 01));
    REQUIRE(r.price_from_date(2020_y / 01 / 01) ==
            r.price_from_date(2020_y / 01 / 02));

    // If request the price of a weekend day returns the price of Friday.
    REQUIRE(r.price_from_date(2019_y / 01 / 04) ==
            r.price_from_date(2019_y / 01 / 05));
    REQUIRE(r.price_from_date(2020_y / 01 / 04) ==
            r.price_from_date(2020_y / 01 / 06));
}
