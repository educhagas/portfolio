//
// Created by eduar on 06/04/2021.
//

#ifndef PORTFOLIO_MARKET_DATA_MAD_H
#define PORTFOLIO_MARKET_DATA_MAD_H
#include "market_data.h"
#include <ostream>
namespace portfolio {
    class market_data_mad : public market_data {
      public:
        market_data_mad(const std::vector<std::string> &assetList,
                        data_feed &df, const minute_point &startPeriod,
                        const minute_point &endPeriod, timeframe tf,
                        int mad_periods);
        friend std::ostream &operator<<(std::ostream &os,
                                        const market_data_mad &mad);
        std::vector<std::string>::iterator assets_begin();
        std::vector<std::string>::iterator assets_end();
        double risk_of(const std::string &asset);
        double expected_return_of(const std::string &asset);

      private:
        std::vector<std::string> assets_;
        std::map<std::string, double> risk_;
        std::map<std::string, double> expected_return_;
    };
} // namespace portfolio

#endif // PORTFOLIO_MARKET_DATA_MAD_H
