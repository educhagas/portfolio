//
// Created by eduardo on 06/04/2021.
//

#include "market_data_mad.h"
#include <iostream>
#include <numeric>
namespace portfolio {

    market_data_mad::market_data_mad(const std::vector<std::string> &assetList,
                                     data_feed &df,
                                     const minute_point &startPeriod,
                                     const minute_point &endPeriod,
                                     timeframe tf, int mad_periods)
        : market_data(assetList, df, startPeriod, endPeriod, tf) {
        for (auto &i : assets_map_) {
            std::vector<double> cp = close_prices(i.first, mad_periods + 1);
            std::vector<double> ret;
            for (int j = 1; j < cp.size(); ++j) {
                ret.push_back((cp[j] - cp[j - 1]) / cp[j - 1]);
            }
            double mean =
                std::accumulate(ret.begin(), ret.end(), 0.0) / ret.size();
            double mad = 0.0;
            std::for_each(
                ret.begin(), ret.end(),
                [&mad, mean](const double &d) { mad += std::abs(d - mean); });
            mad /= ret.size();
            expected_return_[i.first] = mean;
            risk_[i.first] = mad;
            assets_.push_back(i.first);
        }
    }

    std::ostream &operator<<(std::ostream &os, const market_data_mad &mad) {
        for (auto &a : mad.assets_) {
            os << "Asset: " << a << " - Risk: " << mad.risk_.at(a)
               << " - Return: " << mad.expected_return_.at(a) << "\n";
        }
        return os;
    }
    std::vector<std::string>::iterator market_data_mad::assets_begin() {
        return assets_.begin();
    }
    std::vector<std::string>::iterator market_data_mad::assets_end() {
        return assets_.end();
    }
    double market_data_mad::risk_of(const std::string &asset) {
        return risk_[asset];
    }
    double market_data_mad::expected_return_of(const std::string &asset) {
        return expected_return_[asset];
    }

} // namespace portfolio