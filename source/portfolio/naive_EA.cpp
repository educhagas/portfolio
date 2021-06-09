//
// Created by eduar on 04/06/2021.
//

#include "naive_EA.h"
#include <iostream>
#include <matplot/matplot.h>
#include <numeric>
#include <pareto/matplot/front.h>
#include <random>

namespace portfolio {

    naive_EA::naive_EA(portfolio &p, market_data &m, interval_points &interval,
                       int n_periods)
        : _problem(m), _best_portfolio(p), _interval(interval),
          _n_periods(n_periods),
          // Parameters
          _max_generations(50), _population_size(200), _parents_per_children(2),
          _children_proportion(7), _crossover_probability(0.9),
          _mutation_strength(0.1), _lambda_step(0.05),
          // Data
          _current_generation(0) {
        _mutation_strength = (1.0 / this->_problem.size());
        _pareto_front = pareto::front<double, 2, portfolio>({true, false});
        // Initialize population
        _population.reserve(_population_size);
        for (int i = 0; i < _population_size; ++i) {
            _population.emplace_back(_problem);
        }
    }
    void naive_EA::run() {
        double lambda_value = 0.0;
        while (lambda_value < 1.0) {
            _best_fx = std::numeric_limits<double>::max();
            for (int i = 0; i < _max_generations; ++i) {
                evolutionary_cycle(lambda_value);
            }
            _pareto_front.insert(std::make_pair(
                pareto::front<double, 2, portfolio>::key_type(
                    {_best_risk_return.first, -_best_risk_return.second}),
                _best_portfolio));
            lambda_value += _lambda_step;
        }
        for (const auto &[point, value] : _pareto_front) {
            std::cout << point << " -> " << value << std::endl;
        }
        pareto::plot_front(_pareto_front);
        matplot::save("pareto2.jpg");
    }
    void naive_EA::run(size_t iterations, double lambda_step) {
        double lambda_value = 0.0;
        while (lambda_value <= 1.0) {
            _best_fx = std::numeric_limits<double>::max();
            for (int i = 0; i < iterations; ++i) {
                evolutionary_cycle(lambda_value);
            }

            _pareto_front.insert(std::make_pair(
                pareto::front<double, 2, portfolio>::key_type(
                    {_best_risk_return.first, -_best_risk_return.second}),
                _best_portfolio));
            lambda_value += lambda_step;
        }
        for (const auto &[point, value] : _pareto_front) {
            std::cout << point << " -> " << value << std::endl;
        }
        pareto::plot_front(_pareto_front);
        matplot::save("pareto2.jpg");
    }
    void naive_EA::evolutionary_cycle(double lambda_value) {
        // display_status(lambda_value);
        evaluate(_population, lambda_value);
        std::vector<size_t> parent_position =
            selection(_population, n_of_selection_candidates(),
                      selection_strategy::uniform);
        std::vector<individual> children =
            reproduction(_population, parent_position);
        evaluate(children, lambda_value);
        std::vector<size_t> children_position = selection(
            _population, _population_size, selection_strategy::truncate);
        this->_population = update_population(children, children_position);
    }
    void naive_EA::evaluate(std::vector<individual> &population,
                            double lambda_value) {
        for (individual &item : population) {
            // item.fx = item.evaluate(_problem, _interval,_n_periods);
            std::pair<double, double> risk_return =
                item.evaluate(_problem, _interval, _n_periods);
            item.risk = risk_return.first;
            item.expected_return = risk_return.second;
            item.fx = (lambda_value * item.risk) -
                      ((1.0 - lambda_value) * item.expected_return);
            if (item.fx < _best_fx) {
                _best_portfolio = item.s;
                _best_risk_return =
                    std::make_pair(item.risk, item.expected_return);
                _best_fx = item.fx;
            }
        }
    }
    std::vector<size_t> naive_EA::selection(std::vector<individual> &population,
                                            size_t n_of_candidates,
                                            naive_EA::selection_strategy s) {
        switch (s) {
        case selection_strategy::uniform: {
            static std::default_random_engine generator;
            std::vector<size_t> parent_position(n_of_candidates);
            std::uniform_int_distribution<size_t> pos_d(0,
                                                        population.size() - 1);
            for (size_t &position : parent_position) {
                position = pos_d(generator);
            }
            return parent_position;
        }
        case selection_strategy::truncate: {
            std::vector<size_t> parent_position(n_of_candidates);
            std::partial_sort(
                population.begin(), population.begin() + parent_position.size(),
                population.end(),
                [](individual &a, individual &b) { return a.fx > b.fx; });
            std::iota(parent_position.begin(), parent_position.end(), 0);
            return parent_position;
        }
        }
    }
    std::vector<naive_EA::individual>
    naive_EA::reproduction(std::vector<individual> &population,
                           std::vector<size_t> &parent_position) {
        static std::default_random_engine generator;
        std::uniform_real_distribution<double> r(0.0, 1.0);
        std::vector<individual> children;
        for (int j = 0; j < parent_position.size(); j += 2) {
            if (r(generator) < _crossover_probability) {
                // Crossover
                children.push_back(population[parent_position[j]].s.crossover(
                    _problem, population[parent_position[j + 1]].s));
            } else {
                // Mutation
                children.push_back(population[parent_position[j]]);
                children.back().s.mutation(_problem, _mutation_strength);
            }
        }
        return children;
    }
    std::vector<naive_EA::individual>
    naive_EA::update_population(std::vector<individual> &population,
                                std::vector<size_t> &positions) {
        std::vector<individual> r;
        r.reserve(population.size());
        for (size_t position : positions) {
            r.push_back(population[position]);
        }
        return r;
    }
    size_t naive_EA::n_of_selection_candidates() {
        return _population_size * _parents_per_children * _children_proportion;
    }
    void naive_EA::display_status(double lambda_value) {
        std::cout << "Generation #" << ++_current_generation;
        std::cout << " - Lambda value: " << lambda_value;
        std::cout << " - Best_fx: " << this->best_fx() << std::endl;
    }
    double naive_EA::best_fx() { return _best_fx; }

} // namespace portfolio
