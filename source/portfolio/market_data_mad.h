//
// Created by eduar on 06/04/2021.
//

#ifndef PORTFOLIO_MARKET_DATA_MAD_H
#define PORTFOLIO_MARKET_DATA_MAD_H
#include "market_data.h"
namespace portfolio {
    class market_data_mad : public market_data {
      public:
        market_data_mad(const std::vector<std::string> &assetList,
                        const minute_point &startPeriod,
                        const minute_point &endPeriod, timeframe tf,
                        const int mad_periods);
        void disp();
        std::vector<std::string> assets;
        std::map<std::string, double> risk;
        std::map<std::string, double> expected_return;
    };
} // namespace portfolio

#endif // PORTFOLIO_MARKET_DATA_MAD_H
