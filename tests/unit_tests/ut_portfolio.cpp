#define CATCH_CONFIG_MAIN

#include "portfolio/common/algorithm.h"
#include "portfolio/data_feed/alphavantage_data_feed.h"
#include "portfolio/data_feed/mock_data_feed.h"
#include "portfolio/market_data.h"
#include "portfolio/portfolio.h"
#include <catch2/catch.hpp>
#include <chrono>

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

    SECTION("Market Data") {
        REQUIRE(md.contains("ABEV3.SAO"));
        REQUIRE_FALSE(md.contains("ABEH3.SAO"));
    }
    portfolio::portfolio port(md);
    SECTION("Portfolio") {
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
template <typename problem_t, typename solution_t>
void random_search(std::vector<std::string> assets, portfolio::data_feed &df,
                   portfolio::minute_point start_point,
                   portfolio::minute_point end_point, portfolio::timeframe tf,
                   portfolio::interval_points point, int n_periods) {
    // Problem parameters
    std::cout.precision(5);

    // Create problem
    problem_t problem(assets, df, start_point, end_point, tf);
    std::cout << problem << std::endl;

    // Search parameters
    const size_t max_iterations = 1000;

    // Get statistics
    double max_risk = std::numeric_limits<double>::min();
    double min_risk = std::numeric_limits<double>::max();
    double avg_risk = 0.0;

    double max_return = std::numeric_limits<double>::min();
    double min_return = std::numeric_limits<double>::max();
    double avg_return = 0.0;

    // Random search
    for (size_t i = 0; i < max_iterations; ++i) {
        solution_t candidate_solution(problem);
        std::cout << "Random solution #" << i + 1 << std::endl;
        std::cout << candidate_solution
                  << std::endl; // candidate_solution.disp(problem);
        std::pair<double, double> risk_return = candidate_solution.evaluate_mad(
            problem, point,
            n_periods); // double f = candidate_solution.evaluate(problem);
        double f_risk = risk_return.first;
        double f_return = risk_return.second;
        std::cout << "Fitness: Risco: " << f_risk << " Retorno: " << f_return
                  << std::endl
                  << std::endl;
        avg_risk += f_risk;
        avg_return += f_return;
        max_risk = std::max(max_risk, f_risk);
        min_risk = std::min(min_risk, f_risk);
        max_return = std::max(max_return, f_return);
        min_return = std::min(min_return, f_return);
    }

    // Print final statistics
    std::cout << "Max risk for " << typeid(problem).name() << ": " << max_risk
              << std::endl;
    std::cout << "Avg risk for " << typeid(problem).name()
              << avg_risk / max_iterations << std::endl;
    std::cout << "Min risk for " << typeid(problem).name() << ": " << min_risk
              << std::endl
              << std::endl;

    std::cout << "Max return for " << typeid(problem).name() << ": "
              << max_return << std::endl;
    std::cout << "Avg return for " << typeid(problem).name()
              << avg_return / max_iterations << std::endl;
    std::cout << "Min return for " << typeid(problem).name() << ": "
              << min_return << std::endl
              << std::endl;
}
TEST_CASE("Naive EA") {
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
    portfolio::portfolio port(md);
    random_search<portfolio::market_data, portfolio::portfolio>(
        assets, mock_df, mp_start, mp_end, portfolio::timeframe::daily,
        interval, 30);
}