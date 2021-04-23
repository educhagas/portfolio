//
// Created by eduardo on 15/04/2021.
//

#include "portfolio_mad.h"
#include <iostream>
#include <numeric>
#include <random>
namespace portfolio {

    portfolio_mad::portfolio_mad(market_data_mad &mad_data) {
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> d(0.0, 1.0);
        for (auto a = mad_data.assets_begin(); a != mad_data.assets_end();
             ++a) {
            if (0.5 > d(generator)) { // asset is selected or not
                assets_[*a] = d(generator);
            } else {
                assets_[*a] = 0.0;
            }
        }
        normalize_allocation();
        evaluate(mad_data);
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
            static std::default_random_engine generator =
                std::default_random_engine(std::chrono::system_clock::now()
                                               .time_since_epoch()
                                               .count());
            std::uniform_real_distribution<double> d(1.0, 2.0);
            for (auto &[key, value] : assets_) {
                assets_[key] = value * d(generator);
            }
            normalize_allocation();
            return;
        }
    }
    double portfolio_mad::total_allocation() {
        double total;
        total = std::accumulate(
            std::begin(assets_), std::end(assets_), 0.0,
            [](double value,
               const std::map<std::string, double>::value_type &p) {
                return value + p.second;
            });
        return total;
    }
    std::pair<double, double>
    portfolio_mad::evaluate(market_data_mad &mad_data) {
        double total_risk = 0.0;
        double total_return = 0.0;
        for (auto a = mad_data.assets_begin(); a != mad_data.assets_end();
             ++a) {
            total_return += assets_[*a] * mad_data.expected_return_of(*a);
            total_risk += assets_[*a] * mad_data.risk_of(*a);
        }
        total_risk_ = total_risk;
        expected_return_ = total_return;
        return std::make_pair(total_risk, total_return);
    }

    std::ostream &operator<<(std::ostream &os, const portfolio_mad &mad) {
        os << "Asset's allocation:\n";
        for (auto &a : mad.assets_) {
            if (a.second != 0.0) {
                os << a.first << ": " << a.second << "\n";
            }
        }
        os << "Total risk_: " << mad.total_risk_
           << ". Expected return: " << mad.expected_return_ << "\n";
        return os;
    }
} // namespace portfolio