//
// Created by eduar on 15/04/2021.
//

#include "portfolio_mad.h"
#include <iostream>
#include <numeric>
namespace portfolio {
    std::default_random_engine portfolio_mad::generator_ =
        std::default_random_engine(
            std::chrono::system_clock::now().time_since_epoch().count());
    portfolio_mad::portfolio_mad(market_data_mad &mad_data) {
        std::uniform_real_distribution<double> d(0.0, 1.0);
        for (auto &a : mad_data.assets) {
            if (0.5 > d(this->generator_)) { // asset is selected or not
                assets_[a] = d(this->generator_);
            } else {
                assets_[a] = 0.0;
            }
        }
        normalize_allocation();
    }
    void portfolio_mad::normalize_allocation() {
        double total = total_allocation();
        if (total == 1.0) {
            return;
        } else if (total > 1.0) {
            for (auto &[key, value] : assets_) {
                assets_[key] = value / total;
            }
            return;
        } else {
            std::uniform_real_distribution<double> d(1.0, 2.0);
            for (auto &[key, value] : assets_) {
                assets_[key] = value * d(this->generator_);
            }
            normalize_allocation();
            return;
        }
    }
    double portfolio_mad::total_allocation() {
        double total = 0.0;
        for (auto &elem : assets_) {
            total += elem.second;
        }
        return total;
    }
    std::pair<double, double>
    portfolio_mad::evaluate(market_data_mad &mad_data) {
        double total_risk = 0.0;
        double total_return = 0.0;
        for (auto &a : mad_data.assets) {
            total_return += assets_[a] * mad_data.expected_return[a];
            total_risk += assets_[a] * mad_data.risk[a];
        }
        return std::make_pair(total_risk, total_return);
    }
    void portfolio_mad::disp(market_data_mad &mad_data) {
        std::cout << "Asset's allocation:" << std::endl;
        for (auto &a : mad_data.assets) {
            if (assets_[a] != 0.0) {
                std::cout << a << ": " << assets_[a] << std::endl;
            }
        }
        auto risk_return = evaluate(mad_data);
        std::cout << "Total risk: " << risk_return.first
                  << ". Expected return: " << risk_return.second << std::endl;
    }
} // namespace portfolio