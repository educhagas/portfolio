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
    enum class timeframe { daily, weekly, monthly, hourly, minutes_15 };
    class data_feed {
      public:
        virtual data_feed_result
        fetch(std::string_view asset_code,
              std::chrono::time_point<std::chrono::system_clock,
                                      std::chrono::minutes>
                  start_period,
              std::chrono::time_point<std::chrono::system_clock,
                                      std::chrono::minutes>
                  end_period,
              timeframe tf) = 0;
    };
} // namespace portfolio
#endif //PORTFOLIO_DATA_FEED_H