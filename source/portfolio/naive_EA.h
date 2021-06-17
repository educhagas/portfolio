//
// Created by eduar on 04/06/2021. Adapted from Alan de Freitas
//

#ifndef PORTFOLIO_NAIVE_EA_H
#define PORTFOLIO_NAIVE_EA_H
#include "market_data.h"
#include "portfolio.h"
#include <pareto/front.h>

namespace portfolio {
    class naive_EA {
      public:
        enum selection_strategy { uniform, truncate };

      public:
        // Auxiliary class for individuals
        class individual {
          public:
            individual(portfolio rhs) : s(rhs) {}
            individual(market_data rhs) : s(rhs) {}
            std::pair<double, double> evaluate(market_data &rhs,
                                               interval_points &interval,
                                               const int &n_periods) {
                return s.evaluate_mad(rhs, interval, n_periods);
            }
            portfolio s;
            double risk;
            double expected_return;
            double fx;
        };

      public:
        explicit naive_EA(portfolio &p, market_data &m,
                          interval_points &interval, int n_periods);

        void run();

        void run(size_t iterations, double lambda_step);

        [[nodiscard]] double best_fx() const;

      private:
        void evolutionary_cycle(double lambda_value);

        void evaluate(std::vector<individual> &population, double lambda_value);

        [[nodiscard]] size_t n_of_selection_candidates() const;

        std::vector<size_t> selection(std::vector<individual> &population,
                                      size_t n_of_candidates,
                                      selection_strategy s);

        std::vector<individual>
        reproduction(std::vector<individual> &population,
                     std::vector<size_t> &parent_position);

        static std::vector<individual>
        update_population(std::vector<individual> &population,
                          std::vector<size_t> &positions);

        void display_status(double lambda_value);

      private:
        // Search parameters
        size_t _max_generations;
        size_t _population_size;
        size_t _parents_per_children;
        size_t _children_proportion;
        double _crossover_probability;
        double _mutation_strength;
        double _lambda_step;
        interval_points _interval;
        int _n_periods;
        pareto::front<double, 2, portfolio> _pareto_front;

        // Data
        std::vector<individual> _population;
        market_data _problem;
        size_t _current_generation;
        std::vector<portfolio> _result;
        portfolio _best_portfolio;
        std::pair<double, double> _best_risk_return;
        double _best_fx;
        //        // Auxiliary generator
        //        static std::default_random_engine _generator;

        // Get statistics
        // double _best_fx;
    };

} // namespace portfolio
#endif // PORTFOLIO_NAIVE_EA_H
