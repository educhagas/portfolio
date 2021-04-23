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
            assets_map_.insert(std::move(std::make_pair(str, data)));
        }
    }
    std::vector<double> market_data::close_prices(std::string_view asset_code,
                                                  int n_periods) {
        std::string str_asset(asset_code);
        data_feed_result result(assets_map_.at(str_asset));
        std::vector<double> all_closes;
        std::for_each(result.begin(), result.end(),
                      [&all_closes](const price_map::value_type &p) {
                          all_closes.push_back(p.second.close());
                      });
        std::vector<double> closes;

        for (auto it = all_closes.end() - n_periods; it != all_closes.end();
             ++it) {
            closes.push_back(*it);
        }
        return closes;
    }

} // namespace portfolio