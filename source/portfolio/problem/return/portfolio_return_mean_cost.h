//
// Created by eduar on 22/04/2022.
//

#ifndef PORTFOLIO_PORTFOLIO_RETURN_MEAN_COST_H
#define PORTFOLIO_PORTFOLIO_RETURN_MEAN_COST_H

#include "portfolio/market_data.h"
#include "portfolio/portfolio.h"
#include "portfolio/problem/return/portfolio_return.h"

namespace portfolio {

    class portfolio_return_mean_cost : public portfolio_return {
      public:
        portfolio_return_mean_cost(market_data &data, interval_points &interval,
                                   size_t &n_periods,
                                   std::string_view filepath);

        double evaluate_return(
            assets_proportions_iterator assets_proportion_begin,
            assets_proportions_iterator assets_proportion_end) override;

      private:
        std::map<std::string, double> assets_return_;
        std::map<std::string, double> assets_price_;
        std::map<std::string, double> previous_capital_per_asset_;
        portfolio previous_allocation_;
    };

} // namespace portfolio

#endif // PORTFOLIO_PORTFOLIO_RETURN_MEAN_COST_H
