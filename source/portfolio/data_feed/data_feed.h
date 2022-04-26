//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_H
#define PORTFOLIO_DATA_FEED_H

#include "portfolio/core/ohlc_prices.h"
#include "portfolio/data_feed/data_feed_result.h"
#include <chrono>
#include <date/date.h>
#include <string_view>
namespace portfolio {
    using minute_point = std::chrono::time_point<std::chrono::system_clock,
                                                 std::chrono::minutes>;
    using interval_points = std::pair<minute_point, minute_point>;
    using price_map = std::map<interval_points, ohlc_prices>;
    using price_iterator = price_map::const_iterator;
    enum class timeframe { daily, weekly, monthly, hourly, minutes_15 };
    class data_feed {
      public:
        /// \brief Get data and save in data_feed_result.
        /// \param asset_code Symbol of asset.
        /// \param start_period Initial minute_point.
        /// \param end_period Final minute_point.
        /// \param tf Timeframe used on request.
        /// \return Data_feed_result "filled" according to the input parameters.
        virtual data_feed_result fetch(std::string_view asset_code,
                                       minute_point start_period,
                                       minute_point end_period,
                                       timeframe tf) = 0;
    };
} // namespace portfolio
#endif //PORTFOLIO_DATA_FEED_H