//
// Created by Alan Freitas on 12/7/20.
//

#include "data_feed_result.h"
#include <chrono>
namespace portfolio {
    double data_feed_result::current_price() const { return current_price_; }

    double data_feed_result::price(date::year_month_day date) const {
        auto it = historical_data_.find(date);
        // if date is founded
        if (it != historical_data_.end()) {
            return (*it).second;
        } else {
            it = historical_data_.lower_bound(date);
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
                return (*it).second;
            }
        }
    }

    data_feed_result::data_feed_result(
        const std::map<date::year_month_day, double> &historical_data)
        : historical_data_(historical_data) {
        date::year_month_day today =
            floor<date::days>(std::chrono::system_clock::now());
        current_price_ = price(today);
    }
} // namespace portfolio