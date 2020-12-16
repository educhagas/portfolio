//
// Created by Alan Freitas on 12/7/20.
//

#include "data_feed_result.h"
#include <chrono>
namespace portfolio {
    double data_feed_result::current_price() const { return current_price_; }

    data_feed_result::data_feed_result(double current_price)
        : current_price_(current_price) {
        date::year_month_day today =
            floor<date::days>(std::chrono::system_clock::now());
        historical_data_[today] = current_price;
    }

    double data_feed_result::price_from_date(date::year_month_day date) const {
        auto it = historical_data_.lower_bound(date);
        if (historical_data_.end() != historical_data_.find(date) ||
            it == historical_data_.begin()) {
            return (*it).second;
        } else if (it == historical_data_.end()) {
            return historical_data_.rbegin()->second;
        } else {
            it--;
            return (*it).second;
        }
    }

    data_feed_result::data_feed_result(
        const std::map<date::year_month_day, double> &historical_data)
        : historical_data_(historical_data) {
        date::year_month_day today =
            floor<date::days>(std::chrono::system_clock::now());
        current_price_ = price_from_date(today);
    }
} // namespace portfolio