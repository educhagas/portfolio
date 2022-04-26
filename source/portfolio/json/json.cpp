//
// Created by eduar on 09/04/2022.
//

#include "json.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
namespace portfolio {
    void portfolio_to_json(const portfolio &portfolio,
                           std::string_view filepath) {
        nlohmann::json j_to_save;
        for (auto a = portfolio.assets_proportions_begin();
             a != portfolio.assets_proportions_end(); ++a)
            j_to_save["assets"][a->first] = {
                {"proportion", a->second},
                {"price", portfolio.asset_price(a->first)}};
        j_to_save["capital"] = portfolio.capital();
        j_to_save["lower_bound"] = portfolio.lower_bound();
        j_to_save["upper_bound"] = portfolio.upper_bound();
        j_to_save["k"] = portfolio.k();
        std::string fp{filepath};
        std::ofstream fout(fp);
        fout << j_to_save.dump();
        fout.close();
    }
    portfolio from_json(std::string_view filepath) {
        portfolio portfolio;
        std::string line;
        std::string fp{filepath};
        std::ifstream fin(fp);
        if (fin.is_open()) {
            std::getline(fin, line);
            fin.close();
        } else {
            throw std::runtime_error("Cannot open file: " + fp);
        }
        nlohmann::json json_from_file = nlohmann::json::parse(line);
        std::map<std::string, double> proportions;
        std::map<std::string, double> prices;
        for (nlohmann::json::iterator it = json_from_file["assets"].begin();
             it != json_from_file["assets"].end(); ++it) {
            proportions[it.key()] = it.value()["proportion"].get<double>();
            prices[it.key()] = it.value()["price"].get<double>();
        }
        portfolio.capital(json_from_file["capital"].get<double>());
        portfolio.lower_bound(json_from_file["lower_bound"].get<double>());
        portfolio.upper_bound(json_from_file["upper_bound"].get<double>());
        portfolio.k(json_from_file["k"].get<size_t>());
        portfolio.assets_proportions(proportions);
        portfolio.mean_assets_prices(prices);
        return portfolio;
    }

} // namespace portfolio
