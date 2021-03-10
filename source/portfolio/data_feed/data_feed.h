//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_H
#define PORTFOLIO_DATA_FEED_H

#include <chrono>
#include <date/date.h>
#include <portfolio/data_feed/data_feed_result.h>
#include <string_view>

namespace portfolio {
    using day_point =
        std::chrono::time_point<std::chrono::system_clock, date::days>;
    using minute_point = std::chrono::time_point<std::chrono::system_clock,
                                                 std::chrono::minutes>;
    using interval_points = std::pair<minute_point, minute_point>;
    using ohlc_prices = std::tuple<double, double, double, double>;
    using price_map = std::map<interval_points, ohlc_prices>;
    using price_iterator = price_map::iterator;
    enum class timeframe { daily, weekly, monthly, hourly, minutes_15 };
    class data_feed {
      public:
        virtual data_feed_result fetch(std::string_view asset_code,
                                       minute_point start_period,
                                       minute_point end_period,
                                       timeframe tf) = 0;
    };
} // namespace portfolio
#endif //PORTFOLIO_DATA_FEED_H