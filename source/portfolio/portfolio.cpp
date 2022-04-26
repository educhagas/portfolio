//
// Created by Alan Freitas on 12/7/20.
//

#include "portfolio.h"
#include "portfolio/common/algorithm.h"
#include <iostream>
#include <map>
#include <random>
#include <range/v3/numeric/accumulate.hpp>
#include <vector>
namespace portfolio {

    bool portfolio::invariants() const {
        size_t k = 0;
        bool in_bounds = true;
        for (auto &[key, value] : this->assets_proportions_) {
            if (!almost_equal(value, 0.0)) {
                k++;
                if (value < this->lower_bound_ || value > this->upper_bound_) {
                    in_bounds = false;
                }
            }
        }
        return almost_equal(this->total_allocation(), 1.0) && this->k_ == k &&
               in_bounds;
    }
    portfolio::portfolio(const problem &problem)
        : lower_bound_(0.07), upper_bound_(0.13), k_(10), capital_(30000.00) {
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> d_real(lower_bound_,
                                                      upper_bound_);
        std::uniform_int_distribution<int> d_int(1, int(problem.n_assets()));
        std::vector<std::string> selected_assets;
        int aux = 0;
        for (auto a = problem.assets_prices_begin();
             a != problem.assets_prices_end(); ++a) {
            this->assets_proportions_[a->first] = 0.0;
            this->mean_assets_prices_[a->first] = 0.0;
        }
        while (selected_assets.size() < k_) {
            for (auto a = problem.assets_prices_begin();
                 a != problem.assets_prices_end(); ++a) {
                if (d_int(generator) <=
                    problem.n_assets() / k_) { // asset is selected or not
                    if (std::find(selected_assets.begin(),
                                  selected_assets.end(),
                                  a->first) == selected_assets.end()) {
                        assets_proportions_[a->first] = d_real(generator);
                        mean_assets_prices_[a->first] = a->second;
                        selected_assets.push_back(a->first);
                        if (selected_assets.size() == k_) {
                            break;
                        }
                    }
                } else {
                    if (aux == 0) {
                        assets_proportions_[a->first] = 0.0;
                    }
                }
            }
            if (selected_assets.size() < k_) {
                aux++;
            }
        }
        normalize_allocation();
    }
    void portfolio::normalize_allocation() {
        for (auto &[key, value] : assets_proportions_) {
            if (!almost_equal(value, 0.0)) {
                while (value < lower_bound_ || value > upper_bound_) {
                    if (value < lower_bound_) {
                        value = value + 2 * (lower_bound_ - value);
                    } else if (value > upper_bound_) {
                        value = value - 2 * (value - upper_bound_);
                    }
                }
                assets_proportions_[key] = value;
            }
        }
        double total = total_allocation();
        if (almost_equal(total, 1.0, 5)) {
            return;
        } else if (total > 1.0) {
            std::transform(
                assets_proportions_.begin(), assets_proportions_.end(),
                std::inserter(assets_proportions_, assets_proportions_.begin()),
                [&total](auto &p) {
                    p.second = p.second / total;
                    return p;
                });
        } else {
            for (auto &[key, value] : assets_proportions_) {
                if (!almost_equal(value, 0.0)) {
                    assets_proportions_[key] = value / total;
                }
            }
        }
        normalize_allocation();
    }
    double portfolio::total_allocation() const {
        double total;
        total = ranges::accumulate(
            std::begin(assets_proportions_), std::end(assets_proportions_), 0.0,
            [](double value,
               const std::map<std::string, double>::value_type &p) {
                return value + p.second;
            });
        return total;
    }
    std::pair<double, double> portfolio::evaluate(problem &problem) {
        //        if (mad_) {
        //            if (mad_->n_periods() != data.n_periods() ||
        //                mad_->interval() != data.interval()) {
        //                mad_.reset();
        //                mad_.emplace(data);
        //            }
        //        } else {
        //            mad_.emplace(data);
        //        }
        //        double total_risk = 0.0;
        //        double total_return = 0.0;
        //        for (auto &a : assets_proportions_) {
        //            total_risk += a.second * mad_->risk(a.first);
        //            total_return += a.second * mad_->expected_return(a.first);
        //        }
        //        return std::make_pair(total_risk, total_return);
        return problem.evaluate(this->assets_proportions_begin(),
                                this->assets_proportions_end());
    }
    std::ostream &operator<<(std::ostream &os, const portfolio &portfolio1) {
        os << "Assets allocations:\n";
        //        for (auto &a : portfolio1.assets_proportions_) {
        //            if (!almost_equal(a.second, 0.0)) {
        //                os << "Asset: " << a.first << " - Allocation " <<
        //                a.second; if (portfolio1.mad_) {
        //                    os << " - Expect return: "
        //                       << portfolio1.mad_->expected_return(a.first);
        //                    os << " - Risk: " <<
        //                    portfolio1.mad_->risk(a.first);
        //                }
        //                os << "\n";
        //            }
        //        }
        return os;
    }
    void portfolio::mutation(problem &p, double mutation_strength) {
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> d(0, mutation_strength);
        for (auto &[key, value] : assets_proportions_) {
            if (!almost_equal(value, 0.0)) {
                this->assets_proportions_[key] = value + d(generator);
            }
        }
        this->normalize_allocation();
    }
    portfolio portfolio::crossover(const problem &problem, portfolio &rhs) {
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        portfolio child(problem);
        std::vector<std::string> assets_this;
        std::vector<std::string> assets_rhs;
        std::vector<std::string> assets_total;
        std::vector<std::string> chosen_assets;
        std::map<std::string, double> prices;
        for (auto &[key, value] : this->assets_proportions_) {
            if (!almost_equal(value, 0.0)) {
                assets_this.push_back(key);
                prices[key] = this->mean_assets_prices_[key];
            }
            if (!almost_equal(rhs.assets_proportions_[key], 0.0)) {
                assets_rhs.push_back(key);
                prices[key] = rhs.mean_assets_prices_[key];
            }
            child.assets_proportions_[key] = 0.0;
            child.mean_assets_prices_[key] = 0.0;
        }
        std::set_union(assets_this.begin(), assets_this.end(),
                       assets_rhs.begin(), assets_rhs.end(),
                       std::back_inserter(assets_total));
        std::sample(assets_total.begin(), assets_total.end(),
                    std::back_inserter(chosen_assets), k_,
                    std::mt19937{std::random_device{}()});
        std::uniform_real_distribution<double> d(0., 1.);
        double alpha = d(generator);
        for (auto &a : chosen_assets) {
            child.assets_proportions_[a] =
                alpha * this->assets_proportions_.at(a) +
                (1 - alpha) * rhs.assets_proportions_.at(a);
            child.mean_assets_prices_[a] = prices[a];
        }
        child.normalize_allocation();
        return child;
    }
    double portfolio::distance(problem &problem, portfolio &rhs,
                               double max_dist) {
        double total = 0.0;
        for (auto &[key, _] : assets_proportions_) {
            total += std::abs(rhs.assets_proportions_.at(key) -
                              assets_proportions_.at(key));
            if (total > max_dist) {
                return total;
            }
        }
        return total;
    }
    portfolio portfolio::crossover_blx(const problem &problem, portfolio &rhs) {

        portfolio child(problem);
        std::vector<std::string> assets_new;
        std::vector<std::string> assets_remove;
        std::vector<double> assets_prorp;

        for (auto &[key, value] : child.assets_proportions_) {
            child.assets_proportions_[key] =
                (0.5 * this->assets_proportions_[key]) +
                (0.5 * rhs.assets_proportions_[key]);
            if (!almost_equal(child.assets_proportions_[key], 0.0)) {
                assets_new.push_back(key);
                assets_prorp.push_back(child.assets_proportions_.at(key));
            }
        }
        int k = assets_new.size();
        while (k > this->k_) {
            int index =
                std::min_element(assets_prorp.begin(), assets_prorp.end()) -
                assets_prorp.begin();
            assets_prorp.erase(assets_prorp.begin() + index);
            assets_remove.push_back(assets_new.at(index));
            assets_new.erase(assets_new.begin() + index);
            k--;
        }
        for (auto &a : assets_remove) {
            child.assets_proportions_[a] = 0.0;
        }
        child.normalize_allocation();
        return child;
    }
    double portfolio::asset_proportion(std::string asset) const {
        return assets_proportions_.at(asset);
    }
    std::map<std::string, double>::const_iterator
    portfolio::assets_proportions_begin() const {
        return this->assets_proportions_.cbegin();
    }
    std::map<std::string, double>::const_iterator
    portfolio::assets_proportions_end() const {
        return this->assets_proportions_.cend();
    }
    double portfolio::asset_price(std::string asset) const {
        return this->mean_assets_prices_.at(asset);
    }
    double portfolio::capital() const { return this->capital_; }
    double portfolio::lower_bound() const { return this->lower_bound_; }
    double portfolio::upper_bound() const { return this->upper_bound_; }
    size_t portfolio::k() const { return this->k_; }
    portfolio::portfolio() {}
    void portfolio::capital(double capital) { this->capital_ = capital; }
    void portfolio::lower_bound(double lower_bound) {
        this->lower_bound_ = lower_bound;
    }
    void portfolio::upper_bound(double upper_bound) {
        this->upper_bound_ = upper_bound;
    }
    void portfolio::k(size_t k) { this->k_ = k; }
    void portfolio::assets_proportions(
        const std::map<std::string, double> &assets_proportions) {
        this->assets_proportions_.clear();
        this->assets_proportions_ = assets_proportions;
    }
    void portfolio::mean_assets_prices(
        const std::map<std::string, double> &mean_assets_prices) {
        this->mean_assets_prices_.clear();
        this->mean_assets_prices_ = mean_assets_prices;
    }

} // namespace portfolio