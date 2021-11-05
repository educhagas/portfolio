//
// Created by eduar on 26/10/2021.
//

#include "problem.h"

#include <utility>

namespace portfolio {

    std::pair<double, double>
    problem::evaluate(assets_proportions_iterator assets_proportion_begin,
                      assets_proportions_iterator assets_proportion_end) {
        double risk = risk_->evaluate_risk(assets_proportion_begin,
                                           assets_proportion_end);
        double ret = return_->evaluate_return(assets_proportion_begin,
                                              assets_proportion_end);
        return std::make_pair(risk, ret);
    }
    problem::problem(std::shared_ptr<portfolio_risk> risk,
                     std::shared_ptr<portfolio_return> rate,
                     const market_data &data, interval_points interval)
        : risk_(std::move(risk)), return_(std::move(rate)),
          interval_(std::move(interval)) {
        for (auto a = data.assets_map_begin(); a != data.assets_map_end();
             ++a) {
            auto p = a->second.find_prices_from(interval);
            this->assets_prices_[a->first] = p->second.close();
        }
    }
    assets_iterator problem::assets_prices_begin() const {
        return this->assets_prices_.cbegin();
    }
    assets_iterator problem::assets_prices_end() const {
        return this->assets_prices_.cend();
    }
    const interval_points &problem::interval() const { return this->interval_; }
    size_t problem::n_assets() const { return this->assets_prices_.size(); }
} // namespace portfolio
