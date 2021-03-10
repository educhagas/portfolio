//
// Created by Alan Freitas on 12/7/20.
//

#include "data_feed_result.h"
#include <chrono>
#include <utility>
namespace portfolio {

    data_feed_result::data_feed_result(price_map historical_data)
        : historical_data_(std::move(historical_data)) {}

    price_iterator data_feed_result::end() { return historical_data_.end(); }

    bool data_feed_result::empty() { return historical_data_.empty(); }

    [[maybe_unused]] ohlc_prices data_feed_result::lastest_prices2() const {
        // return last price in historical data
        return historical_data_.rbegin()->second;
    }

    price_iterator
    data_feed_result::find_prices_from(interval_points interval) {
        return historical_data_.find(interval);
    }

    ohlc_prices data_feed_result::closest_prices(minute_point date_time) const {
        if (date_time <= historical_data_.begin()->first.first) {
            return historical_data_.begin()->second;
        } else if (date_time >= historical_data_.rbegin()->first.second) {
            return historical_data_.rbegin()->second;
        }
        auto it = historical_data_.begin();
        auto previous_it = it;
        while (date_time > it->first.second) {
            previous_it = it;
            it++;
        }
        if (date_time >= it->first.first && date_time <= it->first.second) {
            return it->second;
        } else {
            return previous_it->second;
        }
    }
} // namespace portfolio