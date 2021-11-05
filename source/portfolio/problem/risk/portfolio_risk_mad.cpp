//
// Created by eduar on 26/10/2021.
//

#include "portfolio_risk_mad.h"
#include <range/v3/numeric/accumulate.hpp>
namespace portfolio {
    double portfolio_risk_mad::evaluate_risk(
        assets_proportions_iterator assets_proportion_begin,
        assets_proportions_iterator assets_proportion_end) {
        double risk = 0.0;
        for (auto a = assets_proportion_begin; a != assets_proportion_end;
             ++a) {
            risk += this->assets_risk_.at(a->first) * a->second;
        }
        return risk;
    }
    portfolio_risk_mad::portfolio_risk_mad(const market_data &data,
                                           const interval_points &interval,
                                           const size_t &n_periods) {
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
            double mad = ranges::accumulate(asset_returns, 0.0,
                                            [mean](double mad, double d) {
                                                return mad + std::abs(d - mean);
                                            }) /
                         asset_returns.size();
            assets_risk_[a->first] = mad;
        }
    }
} // namespace portfolio