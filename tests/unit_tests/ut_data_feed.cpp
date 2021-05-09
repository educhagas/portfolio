#define CATCH_CONFIG_MAIN

#include "portfolio/common/algorithm.h"
#include "portfolio/data_feed/alphavantage_data_feed.h"
#include "portfolio/data_feed/mock_data_feed.h"
#include "portfolio/market_data.h"
#include "portfolio/portfolio.h"
#include <catch2/catch.hpp>
#include <chrono>
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

    interval_points interval;

    // if request the price of nonexistent data and time returns
    // historical_data.end()
    SECTION("NONEXISTENT DATA") {
        mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 0min;
        mp_end = date::sys_days{2019_y / 01 / 01} + 18h + 0min;
        interval = std::make_pair(mp_start, mp_end);
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
    }

    // If request the closest price of a date before the oldest date returns the
    // price of the oldest period.
    SECTION("BEFORE OLDEST PERIOD") {
        REQUIRE(
            r_daily.closest_prices(date::sys_days{2018_y / 12 / 31} + 10h) ==
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 01} + 11h));

        REQUIRE(
            r_daily.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_daily.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

        REQUIRE(
            r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 01} + 9h) ==
            r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h +
                                       01min));

        REQUIRE(
            r_minutes15.closest_prices(date::sys_days{2018_y / 12 / 30} +
                                       10h) ==
            r_minutes15.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

        REQUIRE(
            r_weekly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h));

        REQUIRE(
            r_weekly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

        REQUIRE(
            r_monthly.closest_prices(date::sys_days{2018_y / 12 / 31} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h));

        REQUIRE(
            r_monthly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));

        REQUIRE(
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 01} + 9h) ==
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 01} + 10h));

        REQUIRE(
            r_hourly.closest_prices(date::sys_days{2018_y / 12 / 30} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2018_y / 12 / 29} + 10h));
    }

    // If request the closest price of a date after the newest date returns the
    // price of the newest period.
    SECTION("AFTER NEWEST PERIOD") {
        REQUIRE(
            r_daily.closest_prices(date::sys_days{2019_y / 12 / 31} + 11h) ==
            r_daily.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

        REQUIRE(
            r_daily.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_daily.closest_prices(date::sys_days{2020_y / 01 / 02} + 13h));

        REQUIRE(
            r_minutes15.closest_prices(date::sys_days{2019_y / 12 / 31} +
                                       18h) ==
            r_minutes15.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

        REQUIRE(
            r_minutes15.closest_prices(date::sys_days{2020_y / 01 / 01} +
                                       10h) ==
            r_minutes15.closest_prices(date::sys_days{2020_y / 01 / 02} + 10h));

        REQUIRE(
            r_weekly.closest_prices(date::sys_days{2020_y / 01 / 03} + 18h) ==
            r_weekly.closest_prices(date::sys_days{2020_y / 01 / 05} + 10h));

        REQUIRE(
            r_weekly.closest_prices(date::sys_days{2020_y / 01 / 05} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2020_y / 01 / 04} + 10h));

        REQUIRE(
            r_monthly.closest_prices(date::sys_days{2019_y / 12 / 31} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

        REQUIRE(
            r_monthly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_monthly.closest_prices(date::sys_days{2020_y / 01 / 02} + 10h));

        REQUIRE(
            r_hourly.closest_prices(date::sys_days{2019_y / 12 / 31} + 18h) ==
            r_hourly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h));

        REQUIRE(
            r_hourly.closest_prices(date::sys_days{2020_y / 01 / 01} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2020_y / 01 / 01} + 13h));
    }

    // If request the closest price of a weekend day returns the price of
    // Friday.
    SECTION("WEEKEND PRICE") {
        REQUIRE(
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 04} + 10h));

        REQUIRE(
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_daily.closest_prices(date::sys_days{2019_y / 01 / 04} + 10h));

        REQUIRE(
            r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 05} +
                                       10h) ==
            r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));

        REQUIRE(
            r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 06} +
                                       10h) ==
            r_minutes15.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));

        REQUIRE(
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 04} + 10h));

        REQUIRE(
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_weekly.closest_prices(date::sys_days{2019_y / 01 / 06} + 10h));

        REQUIRE(
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 05} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));

        REQUIRE(
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 06} + 10h) ==
            r_hourly.closest_prices(date::sys_days{2019_y / 01 / 04} + 18h));
    }

    // if requesting the most recent price returns the most recent price in
    // historical
    SECTION("LATEST PRICES") {
        mp_start = date::sys_days{2019_y / 12 / 31} + 10h + 00min;
        mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
        interval = std::make_pair(mp_start, mp_end);
        REQUIRE(r_daily.latest_prices() ==
                r_daily.find_prices_from(interval)->second);

        mp_start = date::sys_days{2019_y / 12 / 31} + 17h + 45min;
        mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
        interval = std::make_pair(mp_start, mp_end);
        REQUIRE(r_minutes15.latest_prices() ==
                r_minutes15.find_prices_from(interval)->second);

        mp_start = date::sys_days{2019_y / 12 / 30} + 10h + 00min;
        mp_end = date::sys_days{2020_y / 01 / 03} + 18h + 00min;
        interval = std::make_pair(mp_start, mp_end);
        REQUIRE(r_weekly.latest_prices() ==
                r_weekly.find_prices_from(interval)->second);

        mp_start = date::sys_days{2019_y / 12 / 01} + 10h + 00min;
        mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
        interval = std::make_pair(mp_start, mp_end);
        REQUIRE(r_monthly.latest_prices() ==
                r_monthly.find_prices_from(interval)->second);

        mp_start = date::sys_days{2019_y / 12 / 31} + 17h + 00min;
        mp_end = date::sys_days{2019_y / 12 / 31} + 18h + 00min;
        interval = std::make_pair(mp_start, mp_end);
        REQUIRE(r_hourly.latest_prices() ==
                r_hourly.find_prices_from(interval)->second);
    }
}
TEST_CASE("Is_floating") {
    std::string_view valid1("2");
    std::string_view valid2("+2");
    std::string_view valid3("-2");
    std::string_view valid4("2278");
    std::string_view valid5("-2278");
    std::string_view valid6("227.909");
    std::string_view valid7("+227.909");
    std::string_view valid8("-227.909");

    std::string_view invalid1("a");
    std::string_view invalid2("-.9");
    std::string_view invalid3("2a.9");
    std::string_view invalid4("2.9.9");
    std::string_view invalid5("-2.9.9");
    std::string_view invalid6("299.");
    SECTION("VALID_CASES") {
        REQUIRE(portfolio::is_floating(valid1));
        REQUIRE(portfolio::is_floating(valid2));
        REQUIRE(portfolio::is_floating(valid3));
        REQUIRE(portfolio::is_floating(valid4));
        REQUIRE(portfolio::is_floating(valid5));
        REQUIRE(portfolio::is_floating(valid6));
        REQUIRE(portfolio::is_floating(valid7));
        REQUIRE(portfolio::is_floating(valid8));
    }
    SECTION("INVALID_CASES") {
        REQUIRE_FALSE(portfolio::is_floating(invalid1));
        REQUIRE_FALSE(portfolio::is_floating(invalid2));
        REQUIRE_FALSE(portfolio::is_floating(invalid3));
        REQUIRE_FALSE(portfolio::is_floating(invalid4));
        REQUIRE_FALSE(portfolio::is_floating(invalid5));
        REQUIRE_FALSE(portfolio::is_floating(invalid6));
    }
}
TEST_CASE("Alphavantage") {
    using namespace portfolio;
    using namespace date::literals;
    using namespace std::chrono_literals;
    // To test other assets_proportions_ and timeframes, use a valid API key.
    // https://www.alphavantage.co/
    std::string_view api_key("demo");
    alphavantage_data_feed a(api_key);

    minute_point mp_start = date::sys_days{2019_y / 01 / 01} + 10h + 0min;
    minute_point mp_end = date::sys_days{2020_y / 12 / 31} + 18h + 0min;

    // get data from web and save file
    data_feed_result r_weekly =
        a.fetch("IBM", mp_start, mp_end, timeframe::weekly);
    REQUIRE(r_weekly.empty() == false);

    data_feed_result r_monthly =
        a.fetch("IBM", mp_start, mp_end, timeframe::monthly);
    REQUIRE(r_monthly.empty() == false);

    // get second time - data from saved data file
    data_feed_result r_weekly2 =
        a.fetch("IBM", mp_start, mp_end, timeframe::weekly);
    REQUIRE(r_weekly2.empty() == false);

    data_feed_result r_monthly2 =
        a.fetch("IBM", mp_start, mp_end, timeframe::monthly);
    REQUIRE(r_monthly2.empty() == false);
}
TEST_CASE("MARKET_DATA_&_PORTFOLIO") {
    // using namespace portfolio;
    using namespace date::literals;
    using namespace std::chrono_literals;
    std::vector<std::string> assets = {
        "PETR4.SAO", "VALE3.SAO", "ITUB4.SAO", "ABEV3.SAO", "BBDC4.SAO",
        "BBAS3.SAO", "CMIG4.SAO", "ELET3.SAO", "ITSA4.SAO", "MGLU3.SAO"};
    portfolio::minute_point mp_start =
        date::sys_days{2018_y / 01 / 01} + 10h + 0min;
    portfolio::minute_point mp_end =
        date::sys_days{2020_y / 12 / 31} + 18h + 0min;
    portfolio::minute_point start_interval =
        date::sys_days{2020_y / 01 / 06} + 10h + 0min;
    portfolio::minute_point end_interval =
        date::sys_days{2020_y / 01 / 06} + 18h + 0min;
    portfolio::interval_points interval =
        std::make_pair(start_interval, end_interval);
    portfolio::mock_data_feed mock_df;
    portfolio::market_data md(assets, mock_df, mp_start, mp_end,
                              portfolio::timeframe::daily);

    SECTION("MARKET_DATA_CONTAINS") {
        REQUIRE(md.contains("ABEV3.SAO"));
        REQUIRE_FALSE(md.contains("ABEH3.SAO"));
    }
    portfolio::portfolio port(md);
    SECTION("PORTFOLIO_MAD") {
        // The expected return of portfolio needs to be different from 0. Risk
        // needs to be greater than 0.
        auto risk_return = port.evaluate_mad(md, interval, 40);
        REQUIRE(risk_return.first > 0);
        REQUIRE(risk_return.second != 0);
        end_interval = date::sys_days{2020_y / 01 / 01} + 18h + 01min;
        interval = std::make_pair(start_interval, end_interval);
        // If the interval is not valid, it throws an exception and ends the
        // execution.
        REQUIRE_THROWS(port.evaluate_mad(md, interval, 40));
        start_interval = date::sys_days{2018_y / 02 / 01} + 10h + 0min;
        end_interval = date::sys_days{2018_y / 02 / 01} + 18h + 00min;
        interval = std::make_pair(start_interval, end_interval);
        // If the period is greater than the number of previous intervals in
        // market_data, it throws an exception and ends the execution.
        REQUIRE_THROWS(port.evaluate_mad(md, interval, 30));
    }
}