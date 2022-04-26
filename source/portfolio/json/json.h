//
// Created by eduar on 09/04/2022.
//

#ifndef PORTFOLIO_JSON_H
#define PORTFOLIO_JSON_H
#include "portfolio/portfolio.h"
namespace portfolio {
    void portfolio_to_json(const portfolio &portfolio,
                           std::string_view filepath);
    portfolio from_json(std::string_view filepath);
} // namespace portfolio
#endif // PORTFOLIO_JSON_H
