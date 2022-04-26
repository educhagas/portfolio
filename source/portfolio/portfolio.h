//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_PORTFOLIO_H
#define PORTFOLIO_PORTFOLIO_H

#include "portfolio/problem/problem.h"

#include <map>
#include <optional>
#include <ostream>
#include <string>
namespace portfolio {
    using assets_proportions_iterator =
        std::map<std::string, double>::const_iterator;
    class portfolio {
      public:
        portfolio();
        explicit portfolio(const problem &problem);
        /// @brief Evaluate portfolio using MAD as risk measure.
        /// \param problem Representation of problem.
        /// \return Risk and expected return of the portfolio.
        std::pair<double, double> evaluate(problem &problem);
        friend std::ostream &operator<<(std::ostream &os,
                                        const portfolio &portfolio1);

        void mutation(problem &p, double mutation_strength);
        portfolio crossover(const problem &problem, portfolio &rhs);
        portfolio crossover_blx(const problem &problem, portfolio &rhs);
        double distance(problem &problem, portfolio &rhs,
                        double max_dist = std::numeric_limits<double>::max());
        [[nodiscard]] bool invariants() const;
        [[nodiscard]] double asset_proportion(std::string asset) const;
        [[nodiscard]] double asset_price(std::string asset) const;
        [[nodiscard]] assets_proportions_iterator
        assets_proportions_begin() const;
        [[nodiscard]] assets_proportions_iterator
        assets_proportions_end() const;
        [[nodiscard]] double capital() const;
        [[nodiscard]] double lower_bound() const;
        [[nodiscard]] double upper_bound() const;
        [[nodiscard]] size_t k() const;
        void capital(double capital);
        void lower_bound(double lower_bound);
        void upper_bound(double upper_bound);
        void k(size_t k);
        void assets_proportions(
            const std::map<std::string, double> &assets_proportions);
        void mean_assets_prices(
            const std::map<std::string, double> &mean_assets_prices);

      private:
        void normalize_allocation();
        [[nodiscard]] double total_allocation() const;

        // std::optional<portfolio_mad> mad_;
        double capital_;
        std::map<std::string, double> assets_proportions_;
        std::map<std::string, double> mean_assets_prices_;
        double lower_bound_;
        double upper_bound_;
        size_t k_;
    };
} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_H
