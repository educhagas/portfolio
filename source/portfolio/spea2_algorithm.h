//
// Created by eduar on 23/06/2022.
//

#ifndef PORTFOLIO_SPEA2_ALGORITHM_H
#define PORTFOLIO_SPEA2_ALGORITHM_H

#include "portfolio.h"
#include <pareto/front.h>

namespace portfolio {

    class spea2_algorithm {
      public:
        // Auxiliary class for individuals
        class individual_spea2 : public portfolio {
          public:
            /// Initialize a new portfolio making a copy of the portfolio
            /// received as a parameter. \param rhs portfolio used to copy.
            individual_spea2(portfolio &rhs) : portfolio(rhs) {}
            /// Initialize a new random portfolio using market_data.
            /// \param rhs market_data used to initialize a portfolio.
            individual_spea2(problem &rhs) : portfolio(rhs) {}
            /// Initialize a new portfolio making a copy of the portfolio
            /// received as a parameter. \param rhs portfolio used to copy.
            individual_spea2(portfolio rhs) : portfolio(rhs) {}
            void mutation(problem &problem, double mutation_strength);
            individual_spea2 crossover(problem &p, individual_spea2 &rhs);
            double second_order_mutation_strength_ = 0.1;
            double crossover_probability_ = 0.8;
            double mutation_strength_ = 0.1;
            double risk;
            double expected_return;
            double fx;
            double fitness;

            std::pair<double, double> risk_return;
            unsigned raw_fitness;
            unsigned strength;

          private:
            void reflect(double &v, double lb, double ub);
        };

        using solution_spea2_ptr = std::shared_ptr<portfolio>;
        using individual_spea2_ptr =
            std::shared_ptr<spea2_algorithm::individual_spea2>;
        using population_spea2_type = std::vector<individual_spea2_ptr>;

        explicit spea2_algorithm(problem &p);
        void population_size(size_t population_size);
        void max_generations(size_t max_generations);

        void run();

      private:
        pareto::front<double, 2, portfolio> pareto_front_;
        population_spea2_type archive_;
        problem problem_;
        size_t population_size_ = 300;
        size_t max_generations_ = 150;
        population_spea2_type merge(population_spea2_type &archive,
                                    population_spea2_type &population);
        void initialize_population();
        void evaluate(population_spea2_type &population);
        population_spea2_type
        tournament_selection(population_spea2_type &population);

        population_spea2_type reproduction(population_spea2_type &parents);

        void selection(population_spea2_type &parents);

        void truncate(population_spea2_type &population);

        void evolutionary_cycle();
        size_t k_th_;
    };

} // namespace portfolio

#endif // PORTFOLIO_SPEA2_ALGORITHM_H
