//
// Created by eduar on 27/10/2021.
//

#ifndef PORTFOLIO_PORTFOLIO_RETURN_MEAN_H
#define PORTFOLIO_PORTFOLIO_RETURN_MEAN_H

#include "portfolio/market_data.h"
#include "portfolio/problem/return/portfolio_return.h"
namespace portfolio {
    using assets_iterator = std::map<std::string, double>::const_iterator;
    using assets_proportions_iterator =
        std::map<std::string, double>::const_iterator;
    class portfolio_return_mean : public portfolio_return {
      public:
        double evaluate_return(
            assets_proportions_iterator assets_proportion_begin,
            assets_proportions_iterator assets_proportion_end) override;

        portfolio_return_mean(market_data &data, interval_points &interval,
                              size_t &n_periods);

      private:
        std::map<std::string, double> assets_return_;
    };
} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_RETURN_MEAN_H
