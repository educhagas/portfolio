//
// Created by eduar on 27/10/2021.
//

#include "portfolio_return_mean.h"
#include <map>
#include <range/v3/numeric/accumulate.hpp>
namespace portfolio {
    portfolio_return_mean::portfolio_return_mean(market_data &data,
                                                 interval_points &interval,
                                                 size_t &n_periods) {
        for (auto a = data.assets_map_begin(); a != data.assets_map_end();
             ++a) {
            data_feed_result df = a->second;
            auto price_it = df.find_prices_from(interval);
            if (price_it == df.end()) {
                throw std::runtime_error("MAD_PORTFOLIO constructor error: "
                                         "interval not found for asset " +
                                         a->first);
            }
            for (size_t i = 0; i < n_periods; ++i) {
                if (price_it != df.begin())
                    price_it--;
                else
                    throw std::runtime_error(
                        "MAD_PORTFOLIO constructor error: "
                        "n_periods out of market_data for asset: " +
                        a->first);
            }
            std::vector<double> asset_returns;
            for (size_t i = 0; i < n_periods; ++i) {
                double price_0 = price_it->second.close();
                price_it++;
                double price_1 = price_it->second.close();
                asset_returns.push_back((price_1 - price_0) / price_0);
            }
            double mean =
                ranges::accumulate(asset_returns, 0.0) / asset_returns.size();
            assets_return_[a->first] = mean;
        }
    }
    double portfolio_return_mean::evaluate_return(
        assets_proportions_iterator assets_proportion_begin,
        assets_proportions_iterator assets_proportion_end) {
        double result = 0.0;
        for (auto a = assets_proportion_begin; a != assets_proportion_end;
             ++a) {
            result += assets_return_[a->first] * a->second;
        }
        return result;
    }

} // namespace portfolio