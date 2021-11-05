#define CATCH_CONFIG_MAIN

#include "portfolio/common/algorithm.h"
#include "portfolio/data_feed/alphavantage_data_feed.h"
#include "portfolio/data_feed/mock_data_feed.h"
#include "portfolio/evolutionary_algorithm.h"
#include "portfolio/market_data.h"
#include "portfolio/portfolio.h"
#include "portfolio/problem/problem.h"
#include "portfolio/problem/return/portfolio_return.h"
#include "portfolio/problem/return/portfolio_return_mean.h"
#include "portfolio/problem/risk/portfolio_risk_mad.h"
#include "portfolio/random_search.h"
#include <catch2/catch.hpp>
#include <chrono>

void new_portfolio_return_mean(portfolio::market_data &md,
                               portfolio::interval_points &interval,
                               size_t n_periods) {
    portfolio::portfolio_return_mean port_return2(md, interval, n_periods);
}

void new_portfolio_risk_mad(portfolio::market_data &md,
                            portfolio::interval_points &interval,
                            size_t n_periods) {
    portfolio::portfolio_risk_mad port_mad(md, interval, n_periods);
}

TEST_CASE("Portfolio and Market Data") {
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
    size_t periods = 12;
    std::shared_ptr<portfolio::portfolio_return_mean> pm(
        new portfolio::portfolio_return_mean(md, interval, periods));
    std::shared_ptr<portfolio::portfolio_risk_mad> port_mad(
        new portfolio::portfolio_risk_mad(md, interval, periods));
    portfolio::problem prob(port_mad, pm, md, interval);

    SECTION("Market Data") {
        REQUIRE(md.contains("ABEV3.SAO"));
        REQUIRE_FALSE(md.contains("ABEH3.SAO"));
    }
    portfolio::portfolio port(prob);
    SECTION("Portfolio") {
        // The expected return of portfolio needs to be different from 0. Risk
        // needs to be greater than 0.
        auto risk_return = port.evaluate(prob);
        REQUIRE(risk_return.first > 0);
        REQUIRE(risk_return.second != 0);

        end_interval = date::sys_days{2020_y / 01 / 01} + 18h + 01min;
        interval = std::make_pair(start_interval, end_interval);
        // If the interval is not valid, it throws an exception and ends the
        // execution.
        periods = 40;
        REQUIRE_THROWS(new_portfolio_return_mean(md, interval, periods));
        REQUIRE_THROWS(new_portfolio_risk_mad(md, interval, periods));

        start_interval = date::sys_days{2018_y / 02 / 01} + 10h + 0min;
        end_interval = date::sys_days{2018_y / 02 / 01} + 18h + 00min;
        interval = std::make_pair(start_interval, end_interval);
        periods = 30;
        // If the period is greater than the number of previous intervals in
        // market_data, it throws an exception and ends the execution.
        REQUIRE_THROWS(new_portfolio_return_mean(md, interval, periods));
        REQUIRE_THROWS(new_portfolio_risk_mad(md, interval, periods));
    }
}