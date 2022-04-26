//
// Created by eduar on 22/04/2022.
//

#include "portfolio_return_mean_cost.h"
#include "portfolio/common/algorithm.h"
#include "portfolio/json/json.h"
#include <map>
#include <range/v3/numeric/accumulate.hpp>

#include "iostream"

namespace portfolio {
    double portfolio_return_mean_cost::evaluate_return(
        assets_proportions_iterator assets_proportion_begin,
        assets_proportions_iterator assets_proportion_end) {
        double result = 0.0;
        std::map<std::string, double> differences;
        std::map<std::string, double> capital_per_asset;
        std::map<std::string, double> allocation_per_asset;
        std::vector<std::string> sell;
        const double transaction_cost = 5.0;
        double sell_volume = 0.0;
        double profits = 0.0;
        for (auto a = assets_proportion_begin; a != assets_proportion_end;
             ++a) {
            // calculate return without costs
            result += assets_return_[a->first] * a->second;
            // calcutate capital per asset to new portfolio
            capital_per_asset[a->first] =
                a->second * previous_allocation_.capital();
            // save allocation per asset to new portfolio
            allocation_per_asset[a->first] = a->second;
            // save assets with buy or sell
            if (!almost_equal((a->second -
                               previous_allocation_.asset_proportion(a->first)),
                              0.0)) {
                differences[a->first] =
                    a->second - previous_allocation_.asset_proportion(a->first);
            }
        }
        for (auto &a : differences) {
            // save assets with sell e sell volume
            if (a.second > 0.0) {
                sell.push_back(a.first);
                sell_volume +=
                    differences.at(a.first) * previous_allocation_.capital();
                profits += (differences.at(a.first) /
                            allocation_per_asset.at(a.first)) *
                           (capital_per_asset.at(a.first) -
                            previous_capital_per_asset_.at(a.first));
            }
        }
        bool has_profit = profits > 0.0;
        bool exceeds_taxable_volume = sell_volume > 20000;
        if (exceeds_taxable_volume && has_profit) {
            result = (1.0 -
                      (differences.size() *
                       (transaction_cost / previous_allocation_.capital())) -
                      ((profits * 0.15) / previous_allocation_.capital())) *
                     result;
        } else {
            result =
                (1.0 - (differences.size() *
                        (transaction_cost / previous_allocation_.capital()))) *
                result;
        }
        return result;
    }
    portfolio_return_mean_cost::portfolio_return_mean_cost(
        market_data &data, interval_points &interval, size_t &n_periods,
        std::string_view filepath) {
        for (auto a = data.assets_map_begin(); a != data.assets_map_end();
             ++a) {
            data_feed_result df = a->second;
            auto price_it = df.find_prices_from(interval);
            assets_price_[a->first] = price_it->second.close();
            if (price_it == df.end()) {
                throw std::runtime_error("MAD_PORTFOLIO constructor error: "
                                         "interval not found for asset " +
                                         a->first);
            }
            for (size_t i = 0; i < n_periods; ++i) {
                if (price_it != df.begin())
                    price_it--;
                else
                    throw std::runtime_error(
                        "MAD_PORTFOLIO constructor error: "
                        "n_periods out of market_data for " +
                        a->first);
            }
            std::vector<double> asset_returns;
            for (size_t i = 0; i < n_periods; ++i) {
                double price_0 = price_it->second.close();
                price_it++;
                double price_1 = price_it->second.close();
                asset_returns.push_back((price_1 - price_0) / price_0);
            }
            double mean =
                ranges::accumulate(asset_returns, 0.0) / asset_returns.size();
            assets_return_[a->first] = mean;
        }
        previous_allocation_ = from_json(filepath);
        std::map<std::string, double> normalized_allocation;
        std::vector<std::string> allocated_assets;
        for (auto it = previous_allocation_.assets_proportions_begin();
             it != previous_allocation_.assets_proportions_end(); ++it) {
            normalized_allocation[it->first] = 0.0;
            previous_capital_per_asset_[it->first] =
                it->second * previous_allocation_.capital();
            if (!almost_equal(it->second, 0.0)) {
                allocated_assets.push_back(it->first);
            }
        }
        std::cout << "SIZE : " << allocated_assets.size() << std::endl;
        std::map<std::string, double> capital_per_asset;
        std::map<std::string, double> difference_per_asset;

        // calculate capital per asset and percentage different of prices
        // between last and new period
        for (std::string &asset : allocated_assets) {
            capital_per_asset[asset] =
                previous_allocation_.capital() *
                previous_allocation_.asset_proportion(asset);
            difference_per_asset[asset] =
                (assets_price_.at(asset) -
                 previous_allocation_.asset_price(asset)) /
                previous_allocation_.asset_price(asset);
        }

        double updated_capital = 0.0;
        // new capital per asset
        for (std::string &asset : allocated_assets) {
            capital_per_asset[asset] = capital_per_asset.at(asset) *
                                       (1.0 + difference_per_asset.at(asset));
            updated_capital += capital_per_asset.at(asset);
        }

        for (std::string &asset : allocated_assets) {
            normalized_allocation[asset] =
                capital_per_asset.at(asset) / updated_capital;
        }
        for (std::string &asset : allocated_assets) {
            std::cout << asset << " - Antes: "
                      << previous_allocation_.asset_proportion(asset)
                      << "- Depois: " << normalized_allocation[asset]
                      << std::endl;
            std::cout << "Preco"
                      << " - Antes: " << previous_allocation_.asset_price(asset)
                      << "- Depois: " << assets_price_.at(asset) << std::endl;
        }
        std::cout << "Capital - Antes: " << previous_allocation_.capital()
                  << "- Depois: " << updated_capital << std::endl;
        previous_allocation_.assets_proportions(normalized_allocation);
        previous_allocation_.capital(updated_capital);
    }
} // namespace portfolio