//
// Created by eduar on 26/10/2021.
//

#ifndef PORTFOLIO_PROBLEM_H
#define PORTFOLIO_PROBLEM_H
#include "portfolio/market_data.h"
#include "portfolio/problem/return/portfolio_return.h"
#include "portfolio/problem/risk/portfolio_risk.h"
namespace portfolio {
    using assets_iterator = std::map<std::string, double>::const_iterator;
    using assets_proportions_iterator =
        std::map<std::string, double>::const_iterator;
    class problem {
      public:
        std::pair<double, double>
        evaluate(assets_proportions_iterator assets_proportion_begin,
                 assets_proportions_iterator assets_proportion_end);
        problem(std::shared_ptr<portfolio_risk> risk,
                std::shared_ptr<portfolio_return> rate, const market_data &data,
                interval_points interval);
        [[nodiscard]] assets_iterator assets_prices_begin() const;
        [[nodiscard]] assets_iterator assets_prices_end() const;
        [[nodiscard]] const interval_points &interval() const;
        [[nodiscard]] size_t n_assets() const;

      private:
        std::map<std::string, double> assets_prices_;
        std::shared_ptr<portfolio_risk> risk_;
        std::shared_ptr<portfolio_return> return_;
        interval_points interval_;
    };
} // namespace portfolio

#endif // PORTFOLIO_PROBLEM_H
