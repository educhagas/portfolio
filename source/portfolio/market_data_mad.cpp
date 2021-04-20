//
// Created by eduar on 06/04/2021.
//

#include "market_data_mad.h"
#include <iostream>
#include <numeric>
namespace portfolio {

    market_data_mad::market_data_mad(const std::vector<std::string> &assetList,
                                     const minute_point &startPeriod,
                                     const minute_point &endPeriod,
                                     timeframe tf, const int mad_periods)
        : market_data(assetList, startPeriod, endPeriod, tf) {
        for (auto &i : asset_list_) {
            std::vector<double> cp = close_prices(i, mad_periods + 1);
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
            expected_return[i] = mean;
            risk[i] = mad;
            assets.push_back(i);
        }
    }
    void market_data_mad::disp() {
        for (auto &a : asset_list_) {
            std::cout << "Asset: " << a << " - Risk: " << risk[a]
                      << " - Return: " << expected_return[a] << std::endl;
        }
    }
} // namespace portfolio