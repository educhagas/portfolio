//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_RESULT_H
#define PORTFOLIO_DATA_FEED_RESULT_H

#include <chrono>
#include <date/date.h>
#include <map>

namespace portfolio {
    using day_point =
        std::chrono::time_point<std::chrono::system_clock, date::days>;
    using minute_point = std::chrono::time_point<std::chrono::system_clock,
                                                 std::chrono::minutes>;
    using interval_points = std::pair<minute_point, minute_point>;
    using ohlc_prices = std::tuple<double, double, double, double>;
    using price_map = std::map<interval_points, ohlc_prices>;
    using price_iterator = price_map::iterator;
    class data_feed_result {
      public /* constructors */:
        explicit data_feed_result(price_map historical_data);
      public /* getters and setters */:
        [[maybe_unused]] [[nodiscard]] ohlc_prices lastest_prices2() const;
        price_iterator find_prices_from(interval_points interval);
        price_iterator end();
        [[nodiscard]] ohlc_prices closest_prices(minute_point date_time) const;
        bool empty();

      private:
        price_map historical_data_;
    };
}

#endif //PORTFOLIO_DATA_FEED_RESULT_H
