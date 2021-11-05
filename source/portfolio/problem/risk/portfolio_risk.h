//
// Created by eduar on 26/10/2021.
//

#ifndef PORTFOLIO_PORTFOLIO_RISK_H
#define PORTFOLIO_PORTFOLIO_RISK_H
#include <map>
#include <string>
namespace portfolio {
    using assets_proportions_iterator =
        std::map<std::string, double>::const_iterator;
    class portfolio_risk {
      public:
        virtual double
        evaluate_risk(assets_proportions_iterator assets_proportion_begin,
                      assets_proportions_iterator assets_proportion_end) = 0;
    };
} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_RISK_H
