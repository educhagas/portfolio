//
// Created by Alan Freitas on 12/7/20.
//

#include "market_data.h"

#include "portfolio/data_feed/alphavantage_data_feed.h"
#include <ranges>
#include <utility>
namespace portfolio {

    market_data::market_data(const std::vector<std::string> &asset_list,
                             data_feed &df, minute_point start_period,
                             minute_point end_period, timeframe tf)
        : data_feed_(df) {
        for (auto &str : asset_list) {
            data_feed_result data =
                data_feed_.fetch(str, start_period, end_period, tf);
            assets_map_.emplace(str, std::move(data));
        }
    }
    std::map<std::string, portfolio::data_feed_result>::const_iterator
    market_data::assets_map_begin() const {
        return assets_map_.cbegin();
    }
    std::map<std::string, portfolio::data_feed_result>::const_iterator
    market_data::assets_map_end() const {
        return assets_map_.cend();
    }
    bool market_data::contains(std::string_view asset) const {
        return assets_map_.find(std::string(asset)) != assets_map_.end();
    }

} // namespace portfolio