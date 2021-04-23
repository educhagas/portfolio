//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_MARKET_DATA_H
#define PORTFOLIO_MARKET_DATA_H
#include "portfolio/data_feed/data_feed.h"
#include "portfolio/data_feed/data_feed_result.h"
#include <map>
#include <string_view>
#include <vector>
namespace portfolio {
    class market_data {
      public:
        market_data(const std::vector<std::string> &asset_list, data_feed &df,
                    minute_point start_period, minute_point end_period,
                    timeframe tf);

        std::vector<double> close_prices(std::string_view asset_code,
                                         int n_periods);

      protected:
        std::map<std::string, portfolio::data_feed_result> assets_map_;

      private:
        data_feed &data_feed_;
    };
} // namespace portfolio

#endif // PORTFOLIO_MARKET_DATA_H
