//
// Created by eduar on 26/10/2021.
//

#ifndef PORTFOLIO_PORTFOLIO_RISK_MAD_H
#define PORTFOLIO_PORTFOLIO_RISK_MAD_H
#include "portfolio/market_data.h"
#include "portfolio/problem/risk/portfolio_risk.h"
namespace portfolio {
    class portfolio_risk_mad : public portfolio_risk {
      public:
        portfolio_risk_mad(const market_data &data,
                           const interval_points &interval,
                           const size_t &n_periods);
        double evaluate_risk(
            assets_proportions_iterator assets_proportion_begin,
            assets_proportions_iterator assets_proportion_end) override;

      private:
        std::map<std::string, double> assets_risk_;
    };
} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_RISK_MAD_H
