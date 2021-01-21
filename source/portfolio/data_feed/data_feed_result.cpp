//
// Created by Alan Freitas on 12/7/20.
//

#include "data_feed_result.h"
#include <chrono>
#include <utility>
namespace portfolio {
    double data_feed_result::lastest_price() const {
        // return last price in historical data
        return historical_data_.rbegin()->second;
    }

    data_feed_result::data_feed_result(
        std::map<std::chrono::time_point<std::chrono::system_clock,
                                         std::chrono::minutes>,
                 double>
            historical_data)
        : historical_data_(std::move(historical_data)) {}
    double data_feed_result::price_from(
        std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>
            date_time) const {
        auto it = historical_data_.find(date_time);
        const bool date_time_is_found = it != historical_data_.end();
        if (date_time_is_found) {
            return it->second;
        } else {
            return -1;
        }
    }
    double data_feed_result::closest_price(
        std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>
            date_time) const {
        auto it = historical_data_.find(date_time);
        const bool date_time_is_found = it != historical_data_.end();
        if (date_time_is_found) {
            return (*it).second;
        } else {
            it = historical_data_.lower_bound(date_time);
            if (it == historical_data_.begin()) {
                // if date is lower than oldest date in historical
                // returns the oldest price in historical
                return historical_data_.cbegin()->second;
            } else if (it == historical_data_.end()) {
                // if the date is higher than the newest date in historical
                // returns the newest price in historical
                return historical_data_.rbegin()->second;
            } else {
                // if the date is weekend day
                // returns the price of the previous day in historical
                it--;
                return it->second;
            }
        }
    }
} // namespace portfolio