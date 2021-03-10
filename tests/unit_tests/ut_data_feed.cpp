#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <chrono>
#include <portfolio/data_feed/alphavantage_data_feed.h>
#include <portfolio/data_feed/mock_data_feed.h>
TEST_CASE("Mock Data Feed") {
    using namespace portfolio;
    using namespace date::literals;
    using namespace std::chrono_literals;
    mock_data_feed m1;

    minute_point mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 0min;
    minute_point mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 0min;

    data_feed_result r_daily1 =
        m1.fetch("PETR4", mp_start, mp_end, timeframe::daily);

    // If start_period is less then end_period historical_data is not empty
    REQUIRE(r_daily1.empty() == false);

    mock_data_feed m2;
    data_feed_result r_daily2 =
        m2.fetch("PETR4", mp_end, mp_start, timeframe::daily);

    // If start_period is greater then end_period historical_data is empty
    REQUIRE(r_daily2.empty() == true);
}
TEST_CASE("Data Feed") {
    using namespace portfolio;
    using namespace date::literals;
    using namespace std::chrono_literals;
    mock_data_feed m;

    minute_point mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 0min;
    minute_point mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 0min;

    data_feed_result r_daily =
        m.fetch("PETR4", mp_start, mp_end, timeframe::daily);

    data_feed_result r_minutes15 =
        m.fetch("PETR4", mp_start, mp_end, timeframe::minutes_15);

    data_feed_result r_weekly =
        m.fetch("PETR4", mp_start, mp_end, timeframe::weekly);

    data_feed_result r_monthly =
        m.fetch("PETR4", mp_start, mp_end, timeframe::monthly);

    data_feed_result r_hourly =
        m.fetch("PETR4", mp_start, mp_end, timeframe::hourly);

    // if request the price of inexistent data and time returns
    // historical_data.end()
    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 0min;
    mp_end = date::sys_days{2019_y / 01 / 01} + 18h + 0min;
    interval_points interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_daily.find_prices_from(interval) != r_daily.end());

    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 01min;
    mp_end = date::sys_days{2019_y / 01 / 01} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_daily.find_prices_from(interval) == r_daily.end());

    mp_start = date::sys_days{2018_y / 12 / 31} + 10h + 00min;
    mp_end = date::sys_days{2018_y / 12 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_daily.find_prices_from(interval) == r_daily.end());

    mp_start = date::sys_days{2020_y / 01 / 01} + 10h + 00min;
    mp_end = date::sys_days{2020_y / 01 / 01} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_daily.find_prices_from(interval) == r_daily.end());

    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 00min;
    mp_end = date::sys_days{2019_y / 01 / 01} + 10h + 15min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_minutes15.find_prices_from(interval) != r_minutes15.end());

    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 01min;
    mp_end = date::sys_days{2019_y / 01 / 01} + 10h + 15min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_minutes15.find_prices_from(interval) == r_minutes15.end());

    mp_start = date::sys_days{2018_y / 12 / 31} + 10h + 00min;
    mp_end = date::sys_days{2018_y / 12 / 31} + 10h + 15min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_minutes15.find_prices_from(interval) == r_minutes15.end());

    mp_start = date::sys_days{2020_y / 01 / 01} + 10h + 00min;
    mp_end = date::sys_days{2020_y / 01 / 01} + 10h + 15min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_minutes15.find_prices_from(interval) == r_minutes15.end());

    mp_start = date::sys_days{2019_y / 01 / 07} + 10h + 00min;
    mp_end = date::sys_days{2019_y / 01 / 11} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_weekly.find_prices_from(interval) != r_weekly.end());

    mp_start = date::sys_days{2019_y / 01 / 07} + 10h + 01min;
    mp_end = date::sys_days{2019_y / 01 / 11} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_weekly.find_prices_from(interval) == r_weekly.end());

    mp_start = date::sys_days{2020_y / 01 / 06} + 10h + 00min;
    mp_end = date::sys_days{2020_y / 01 / 10} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_weekly.find_prices_from(interval) == r_weekly.end());

    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 00min;
    mp_end = date::sys_days{2019_y / 01 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_monthly.find_prices_from(interval) != r_monthly.end());

    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 01min;
    mp_end = date::sys_days{2019_y / 01 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_monthly.find_prices_from(interval) == r_monthly.end());

    mp_start = date::sys_days{2018_y / 12 / 01} + 10h + 00min;
    mp_end = date::sys_days{2018_y / 12 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_monthly.find_prices_from(interval) == r_monthly.end());

    mp_start = date::sys_days{2020_y / 01 / 01} + 10h + 00min;
    mp_end = date::sys_days{2020_y / 01 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_monthly.find_prices_from(interval) == r_monthly.end());

    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 00min;
    mp_end = date::sys_days{2019_y / 01 / 01} + 11h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_hourly.find_prices_from(interval) != r_hourly.end());

    mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 01min;
    mp_end = date::sys_days{2019_y / 01 / 01} + 11h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_hourly.find_prices_from(interval) == r_hourly.end());

    mp_start = date::sys_days{2018_y / 12 / 31} + 10h + 00min;
    mp_end = date::sys_days{2018_y / 12 / 31} + 11h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_hourly.find_prices_from(interval) == r_hourly.end());

    mp_start = date::sys_days{2020_y / 01 / 01} + 10h + 00min;
    mp_end = date::sys_days{2020_y / 01 / 01} + 11h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_hourly.find_prices_from(interval) == r_hourly.end());

    // If request the closest price of a date before the oldest date returns the
    // price of the oldest period.
    REQUIRE(r_daily.closest_prices(date::sys_days{2018_y / 12 / 31} + 10h) ==
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 01} + 11h));

    REQUIRE(r_daily.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_daily.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

    REQUIRE(r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 01} + 9h) ==
            r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h +
                                       01min));

    REQUIRE(
        r_minutes15.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
        r_minutes15.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

    REQUIRE(r_weekly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h));

    REQUIRE(r_weekly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

    REQUIRE(r_monthly.closest_prices(date::sys_days{2018_y / 12 / 31} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h));

    REQUIRE(r_monthly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

    REQUIRE(r_hourly.closest_prices(date::sys_days{2019_y / 01 / 01} + 9h) ==
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h));

    REQUIRE(r_hourly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

    // If request the closest price of a date after the newest date returns the
    // price of the newest period.
    REQUIRE(r_daily.closest_prices(date::sys_days{2019_y / 12 / 31} + 11h) ==
            r_daily.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

    REQUIRE(r_daily.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_daily.closest_prices(date::sys_days{2020_y / 01 / 02} + 13h));

    REQUIRE(
        r_minutes15.closest_prices(date::sys_days{2019_y / 12 / 31} + 18h) ==
        r_minutes15.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

    REQUIRE(
        r_minutes15.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h) ==
        r_minutes15.closest_prices(date::sys_days{2020_y / 01 / 02} + 10h));

    REQUIRE(r_weekly.closest_prices(date::sys_days{2020_y / 01 / 03} + 18h) ==
            r_weekly.closest_prices(date::sys_days{2020_y / 01 / 05} + 10h));

    REQUIRE(r_weekly.closest_prices(date::sys_days{2020_y / 01 / 05} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2020_y / 01 / 04} + 10h));

    REQUIRE(r_monthly.closest_prices(date::sys_days{2019_y / 12 / 31} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

    REQUIRE(r_monthly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2020_y / 01 / 02} + 10h));

    REQUIRE(r_hourly.closest_prices(date::sys_days{2019_y / 12 / 31} + 18h) ==
            r_hourly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

    REQUIRE(r_hourly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2020_y / 01 / 01} + 13h));

    // If request the closest price of a weekend day returns the price of
    // Friday.
    REQUIRE(r_daily.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 04} + 10h));

    REQUIRE(r_daily.closest_prices(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 04} + 10h));

    REQUIRE(
        r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
        r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));

    REQUIRE(
        r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 06} + 10h) ==
        r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));

    REQUIRE(r_weekly.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 04} + 10h));

    REQUIRE(r_weekly.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 06} + 10h));

    REQUIRE(r_hourly.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));

    REQUIRE(r_hourly.closest_prices(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));

    // if requesting the most recent price returns the most recent price in
    // historical

    mp_start = date::sys_days{2019_y / 12 / 31} + 10h + 00min;
    mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_daily.lastest_prices2() ==
            r_daily.find_prices_from(interval)->second);

    mp_start = date::sys_days{2019_y / 12 / 31} + 17h + 45min;
    mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_minutes15.lastest_prices2() ==
            r_minutes15.find_prices_from(interval)->second);

    mp_start = date::sys_days{2019_y / 12 / 30} + 10h + 00min;
    mp_end = date::sys_days{2020_y / 01 / 03} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_weekly.lastest_prices2() ==
            r_weekly.find_prices_from(interval)->second);

    mp_start = date::sys_days{2019_y / 12 / 01} + 10h + 00min;
    mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_monthly.lastest_prices2() ==
            r_monthly.find_prices_from(interval)->second);

    mp_start = date::sys_days{2019_y / 12 / 31} + 17h + 00min;
    mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
    interval = std::make_pair(mp_start, mp_end);
    REQUIRE(r_hourly.lastest_prices2() ==
            r_hourly.find_prices_from(interval)->second);
}
