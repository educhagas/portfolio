//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_PORTFOLIO_H
#define PORTFOLIO_PORTFOLIO_H

#include "market_data.h"
#include "portfolio_mad.h"
#include <map>
#include <optional>
#include <ostream>
#include <string>
namespace portfolio {
    class portfolio {
      public:
        explicit portfolio(const market_data &data);
        /// @brief Evaluate portfolio using MAD as risk measure.
        /// \param data Market data of assets.
        /// \param interval Time interval for which you want to calculate MAD.
        /// \param n_periods Number of time periods used for calculating MAD.
        /// \return Risk and expected return of the portfolio.
        std::pair<double, double> evaluate_mad(const market_data &data,
                                               interval_points interval,
                                               int n_periods);
        friend std::ostream &operator<<(std::ostream &os,
                                        const portfolio &portfolio1);

      private:
        void normalize_allocation();
        [[nodiscard]] double total_allocation() const;
        [[nodiscard]] bool invariants() const;
        std::optional<portfolio_mad> mad_;
        std::map<std::string, double> assets_proportions_;
    };
} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_H
