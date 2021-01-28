#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <chrono>
#include <portfolio/data_feed/mock_data_feed.h>
TEST_CASE("Mock Data Feed"){
    using namespace portfolio;
    using namespace date::literals;
    using namespace std::chrono_literals;
    mock_data_feed m1;
    data_feed_result r_daily =
        m1.fetch("PETR4", date::sys_days{2019_y / 1 /1} + 10h + 0min,
                date::sys_days{2019_y / 12 / 31} + 18h + 0min,
                timeframe::daily);
    //If start_period is less then end_period historical_data is not empty
    REQUIRE(r_daily.historical_data_is_empty() == false);

    mock_data_feed m2;
    data_feed_result r_daily2 =
        m2.fetch("PETR4", date::sys_days{2019_y / 12 / 31} + 10h + 0min,
                 date::sys_days{2019_y / 1 /1} + 18h + 0min,
                 timeframe::daily);
    //If start_period is greater then end_period historical_data is not empty
    REQUIRE(r_daily2.historical_data_is_empty() == true);
}
TEST_CASE("Data Feed") {
    using namespace portfolio;
    using namespace date::literals;
    using namespace std::chrono_literals;
    mock_data_feed m;

    data_feed_result r_daily = m.fetch(
        "PETR4", date::sys_days{2019_y / 1 / 1} + 10h + 0min,
        date::sys_days{2019_y / 12 / 31} + 18h + 0min, timeframe::daily);
    data_feed_result r_minutes15 = m.fetch(
        "PETR4", date::sys_days{2019_y / 1 / 1} + 10h + 0min,
        date::sys_days{2019_y / 12 / 31} + 18h + 0min, timeframe::minutes_15);
    data_feed_result r_weekly = m.fetch(
        "PETR4", date::sys_days{2019_y / 1 / 1} + 10h + 0min,
        date::sys_days{2019_y / 12 / 31} + 18h + 0min, timeframe::weekly);

    data_feed_result r_monthly = m.fetch(
        "PETR4", date::sys_days{2019_y / 1 / 1} + 10h + 0min,
        date::sys_days{2019_y / 12 / 31} + 18h + 0min, timeframe::monthly);

    data_feed_result r_hourly = m.fetch(
        "PETR4", date::sys_days{2019_y / 1 / 1} + 10h + 0min,
        date::sys_days{2019_y / 12 / 31} + 18h + 0min, timeframe::hourly);

    // if request the price of inexistent data and time returns
    // historical_data.end()
    REQUIRE(r_daily.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                    0min) != r_daily.get_historical_end());
    REQUIRE(r_daily.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                    01min) == r_daily.get_historical_end());
    REQUIRE(r_daily.find_price_from(date::sys_days{2018_y / 12 / 31} + 10h +
                                    0min) == r_daily.get_historical_end());
    REQUIRE(r_daily.find_price_from(date::sys_days{2020_y / 01 / 01} + 10h +
                                    0min) == r_daily.get_historical_end());
    REQUIRE(r_minutes15.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                        0min) !=
            r_minutes15.get_historical_end());
    REQUIRE(r_minutes15.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                        01min) ==
            r_minutes15.get_historical_end());
    REQUIRE(r_minutes15.find_price_from(date::sys_days{2018_y / 12 / 31} + 10h +
                                        0min) ==
            r_minutes15.get_historical_end());
    REQUIRE(r_minutes15.find_price_from(date::sys_days{2020_y / 01 / 01} + 10h +
                                        0min) ==
            r_minutes15.get_historical_end());
    REQUIRE(r_weekly.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                     0min) != r_weekly.get_historical_end());
    REQUIRE(r_weekly.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                     01min) == r_weekly.get_historical_end());
    REQUIRE(r_weekly.find_price_from(date::sys_days{2018_y / 12 / 31} + 10h +
                                     0min) == r_weekly.get_historical_end());
    REQUIRE(r_weekly.find_price_from(date::sys_days{2020_y / 01 / 01} + 10h +
                                     0min) == r_weekly.get_historical_end());
    REQUIRE(r_monthly.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                      0min) != r_monthly.get_historical_end());
    REQUIRE(r_monthly.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                      01min) == r_monthly.get_historical_end());
    REQUIRE(r_monthly.find_price_from(date::sys_days{2018_y / 12 / 31} + 10h +
                                      0min) == r_monthly.get_historical_end());
    REQUIRE(r_monthly.find_price_from(date::sys_days{2020_y / 01 / 01} + 10h +
                                      0min) == r_monthly.get_historical_end());
    REQUIRE(r_hourly.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                     0min) != r_hourly.get_historical_end());
    REQUIRE(r_hourly.find_price_from(date::sys_days{2019_y / 01 / 01} + 10h +
                                     01min) == r_hourly.get_historical_end());
    REQUIRE(r_hourly.find_price_from(date::sys_days{2018_y / 12 / 31} + 10h +
                                     0min) == r_hourly.get_historical_end());
    REQUIRE(r_hourly.find_price_from(date::sys_days{2020_y / 01 / 01} + 10h +
                                     0min) == r_hourly.get_historical_end());

    // If request the closest price of a date before the oldest date returns the
    // price of the oldest period.
    REQUIRE(r_daily.closest_price(date::sys_days{2018_y / 12 / 31} + 10h) ==
            r_daily.closest_price(date::sys_days{2019_y / 01 / 01} + 10h));
    REQUIRE(r_daily.closest_price(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_daily.closest_price(date::sys_days{2018_y / 12 / 29} + 10h));
    REQUIRE(r_minutes15.closest_price(date::sys_days{2019_y / 01 / 01} + 9h) ==
            r_minutes15.closest_price(date::sys_days{2019_y / 01 / 01} + 10h));
    REQUIRE(r_minutes15.closest_price(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_minutes15.closest_price(date::sys_days{2018_y / 12 / 29} + 10h));
    REQUIRE(r_weekly.closest_price(date::sys_days{2018_y / 12 / 31} + 10h) ==
            r_weekly.closest_price(date::sys_days{2019_y / 01 / 01} + 10h));
    REQUIRE(r_weekly.closest_price(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_weekly.closest_price(date::sys_days{2018_y / 12 / 29} + 10h));
    REQUIRE(r_monthly.closest_price(date::sys_days{2018_y / 12 / 31} + 10h) ==
            r_monthly.closest_price(date::sys_days{2019_y / 01 / 01} + 10h));
    REQUIRE(r_monthly.closest_price(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_monthly.closest_price(date::sys_days{2018_y / 12 / 29} + 10h));
    REQUIRE(r_hourly.closest_price(date::sys_days{2019_y / 01 / 01} + 9h) ==
            r_hourly.closest_price(date::sys_days{2019_y / 01 / 01} + 10h));
    REQUIRE(r_hourly.closest_price(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_hourly.closest_price(date::sys_days{2018_y / 12 / 29} + 10h));

    // If request the closest price of a date after the newest date returns the
    // price of the newest period.
    REQUIRE(r_daily.closest_price(date::sys_days{2019_y / 12 / 31} + 10h) ==
            r_daily.closest_price(date::sys_days{2020_y / 01 / 01} + 10h));
    REQUIRE(r_daily.closest_price(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_daily.closest_price(date::sys_days{2020_y / 01 / 02} + 10h));
    REQUIRE(r_minutes15.closest_price(date::sys_days{2019_y / 12 / 31} + 18h) ==
            r_minutes15.closest_price(date::sys_days{2020_y / 01 / 01} + 10h));
    REQUIRE(r_minutes15.closest_price(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_minutes15.closest_price(date::sys_days{2020_y / 01 / 02} + 10h));
    REQUIRE(r_weekly.closest_price(date::sys_days{2019_y / 12 / 31} + 10h) ==
            r_weekly.closest_price(date::sys_days{2020_y / 01 / 01} + 10h));
    REQUIRE(r_weekly.closest_price(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_weekly.closest_price(date::sys_days{2020_y / 01 / 02} + 10h));
    REQUIRE(r_monthly.closest_price(date::sys_days{2019_y / 12 / 31} + 10h) ==
            r_monthly.closest_price(date::sys_days{2020_y / 01 / 01} + 10h));
    REQUIRE(r_monthly.closest_price(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_monthly.closest_price(date::sys_days{2020_y / 01 / 02} + 10h));
    REQUIRE(r_hourly.closest_price(date::sys_days{2019_y / 12 / 31} + 18h) ==
            r_hourly.closest_price(date::sys_days{2020_y / 01 / 01} + 10h));
    REQUIRE(r_hourly.closest_price(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_hourly.closest_price(date::sys_days{2020_y / 01 / 02} + 10h));

    // If request the closest price of a weekend day returns the price of
    // Friday.
    REQUIRE(r_daily.closest_price(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_daily.closest_price(date::sys_days{2019_y / 01 / 04} + 10h));
    REQUIRE(r_daily.closest_price(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_daily.closest_price(date::sys_days{2019_y / 01 / 04} + 10h));
    REQUIRE(r_minutes15.closest_price(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_minutes15.closest_price(date::sys_days{2019_y / 01 / 04} + 18h));
    REQUIRE(r_minutes15.closest_price(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_minutes15.closest_price(date::sys_days{2019_y / 01 / 04} + 18h));
    REQUIRE(r_weekly.closest_price(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_weekly.closest_price(date::sys_days{2019_y / 01 / 04} + 10h));
    REQUIRE(r_weekly.closest_price(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_weekly.closest_price(date::sys_days{2019_y / 01 / 04} + 10h));
    REQUIRE(r_monthly.closest_price(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_monthly.closest_price(date::sys_days{2019_y / 01 / 04} + 10h));
    REQUIRE(r_monthly.closest_price(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_monthly.closest_price(date::sys_days{2019_y / 01 / 04} + 10h));
    REQUIRE(r_hourly.closest_price(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_hourly.closest_price(date::sys_days{2019_y / 01 / 04} + 18h));
    REQUIRE(r_hourly.closest_price(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_hourly.closest_price(date::sys_days{2019_y / 01 / 04} + 18h));

    // if requesting the most recent price returns the most recent price in
    // historical
    REQUIRE(r_daily.lastest_price() ==
            r_daily.find_price_from(date::sys_days{2019_y / 12 / 31} + 10h)
                ->second);
    REQUIRE(r_minutes15.lastest_price() ==
            r_minutes15.find_price_from(date::sys_days{2019_y / 12 / 31} + 18h)
                ->second);
    REQUIRE(r_weekly.lastest_price() ==
            r_weekly.find_price_from(date::sys_days{2019_y / 12 / 31} + 10h)
                ->second);
    REQUIRE(r_monthly.lastest_price() ==
            r_monthly.find_price_from(date::sys_days{2019_y / 11 / 07} + 10h)
                ->second);
    REQUIRE(r_hourly.lastest_price() ==
            r_hourly.find_price_from(date::sys_days{2019_y / 12 / 31} + 18h)
                ->second);
}
