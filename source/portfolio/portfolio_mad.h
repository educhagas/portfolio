//
// Created by eduardo on 15/04/2021.
//

#ifndef PORTFOLIO_PORTFOLIO_MAD_H
#define PORTFOLIO_PORTFOLIO_MAD_H

#include "market_data.h"

namespace portfolio {
    class portfolio_mad {
      public:
        portfolio_mad(const market_data &data, interval_points interval,
                      int n_periods);
        [[nodiscard]] interval_points interval() const;
        [[nodiscard]] int n_periods() const;
        [[nodiscard]] double risk(std::string_view asset) const;
        [[nodiscard]] double expected_return(std::string_view asset) const;

      private:
        interval_points interval_;
        int n_periods_;
        std::map<std::string, std::pair<double, double>> assets_risk_return_;
    };

} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_MAD_H
