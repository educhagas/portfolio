//
// Created by Alan Freitas on 12/7/20.
//

#include "mock_data_feed.h"
#include <iostream>
#include <random>

using day_point =
    std::chrono::time_point<std::chrono::system_clock, date::days>;
namespace portfolio {

    data_feed_result mock_data_feed::fetch(
        std::string_view asset_code,
        std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>
            start_period,
        std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>
            end_period,
        timeframe tf) {
        return data_feed_result(
            generate_historical_data(start_period, end_period, tf));
    }
    std::chrono::minutes
    mock_data_feed::calculate_increment(portfolio::timeframe tf) {
        using namespace std::chrono_literals;
        switch (tf) {
        case timeframe::minutes_15:
            return 15min;
        case timeframe::daily:
            return 24h;
        case timeframe::hourly:
            return 60min;
        case timeframe::monthly:
            return 31 * 24h;
        case timeframe::weekly:
            return 7 * 24h;
        default:
            return 24h;
        }
    }
    std::map<std::chrono::time_point<std::chrono::system_clock,
                                     std::chrono::minutes>,
             double>
    mock_data_feed::generate_historical_data(
        std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>
            start_period,
        std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>
            end_period,
        timeframe tf) {
        std::chrono::minutes increment = calculate_increment(tf);
        std::map<std::chrono::time_point<std::chrono::system_clock,
                                         std::chrono::minutes>,
                 double>
            historical;
        static std::random_device r;
        static std::default_random_engine generator(r());
        std::uniform_int_distribution<int> ud_int(10, 50);
        std::uniform_real_distribution<double> ud_double(1, 2);
        // initial price between 10.00 and 100.00
        double current_price = ud_int(generator) * ud_double(generator);
        for (std::chrono::time_point<std::chrono::system_clock,
                                     std::chrono::minutes>
                 i = start_period;
             i <= end_period; i = i + increment) {
            day_point dp = date::floor<date::days>(i);
            using namespace date;
            using namespace std::chrono_literals;
            if (dp != date::Saturday &&
                dp != date::Sunday) { // if is not weekend
                auto time = date::make_time(
                    std::chrono::duration_cast<std::chrono::minutes>(i - dp));
                date::hh_mm_ss<std::chrono::seconds> start_time(10h + 0min);
                date::hh_mm_ss<std::chrono::seconds> end_time(18h + 0min);
                if (time.to_duration() >= start_time.to_duration() &&
                    time.to_duration() <= end_time.to_duration()) {
                    int side = ud_int(generator);
                    double volatility = (ud_double(generator) - 1) * 5;
                    side < 30 ? current_price *= 1 - (volatility / 100)
                              : current_price *= 1 + (volatility / 100);
                    // no negatives prices
                    current_price > 0 ? historical[i] = current_price
                                      : historical[i] = 0;
                    using namespace date;
                    std::cout << i << " : " << current_price << std::endl;
                }
            }
        }
        return historical;
    }

} // namespace portfolio