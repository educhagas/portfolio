//
// Created by eduar on 23/06/2022.
//

#include "spea2_algorithm.h"
#include "portfolio/matplot/pareto_front.h"
#include <random>
#include <ranges>

namespace portfolio {

    spea2_algorithm::spea2_algorithm(problem &p) : problem_(p) {
        this->pareto_front_ =
            pareto::front<double, 2, portfolio>({pareto::min, pareto::max});
    }
    void spea2_algorithm::population_size(size_t population_size) {
        this->population_size_ = population_size;
    }
    void spea2_algorithm::max_generations(size_t max_generations) {
        this->max_generations_ = max_generations;
    }
    void spea2_algorithm::run() {
        this->k_th_ = std::ceil(std::sqrt(2 * this->population_size_));
        initialize_population();
        evaluate(this->archive_);
        std::ofstream fout("spea.txt");
        fout << "{ 0 : \"";
        size_t j = 0;
        for (const individual_spea2_ptr i : this->archive_) {
            fout << 10000 * i->risk_return.first << ","
                 << 10000 * i->risk_return.second;
            if (j < this->archive_.size() - 1) {
                fout << ";";
            }
            j++;
        }
        fout << "\"";
        for (int i = 0; i < this->max_generations_; ++i) {
            fout << "," << i + 1 << " : \"";
            evolutionary_cycle();
            size_t j = 0;
            for (const individual_spea2_ptr i : this->archive_) {
                fout << 10000 * i->risk_return.first << ","
                     << 10000 * i->risk_return.second;
                if (j < this->archive_.size() - 1) {
                    fout << ";";
                }
                j++;
            }
            fout << "\"";
        }
        fout << "}";
        fout.close();
        for (individual_spea2_ptr &item : this->archive_) {
            pareto_front_.insert(std::make_pair(
                pareto::front<double, 2, portfolio>::key_type(
                    {item->risk_return.first, item->risk_return.second}),
                *item));
        }
        save_plot(this->pareto_front_, "plotspea.jpg");
        //        std::cout << "=============evaluate begin" << std::endl;
        //        for (individual_spea2_ptr i : this->archive_){
        //            std::cout << 1000 * i->risk_return.first << " - " << 1000
        //            * i->risk_return.second << " - raw: " << i->raw_fitness <<
        //            " - s: "<< i->strength << " - fit: "<< i->fitness<<
        //            std::endl;
        //        }
        //        std::cout << "=============evaluate end" << std::endl;
    }
    void spea2_algorithm::initialize_population() {
        // this->population_.resize(0);
        this->archive_.resize(0);
        // this->population_.reserve(this->population_size_);
        this->archive_.reserve(this->population_size_);
        for (int i = 0; i < this->population_size_; ++i) {
            // this->population_.emplace_back(
            //     std::make_shared<individual_spea2>(this->problem_));
            this->archive_.emplace_back(
                std::make_shared<individual_spea2>(this->problem_));
        }
    }
    void spea2_algorithm::evolutionary_cycle() {
        population_spea2_type parents = tournament_selection(this->archive_);
        // std::cout<<"parents size: "<<parents.size() <<std::endl;
        population_spea2_type children = reproduction(parents);
        // std::cout<<"children size: "<<children.size() <<std::endl;

        population_spea2_type parents_and_children =
            this->merge(this->archive_, children);

        // std::cout<<"parents and clidren size: "<<parents_and_children.size()
        // <<std::endl;
        evaluate(parents_and_children);

        selection(parents_and_children);
    }

    spea2_algorithm::population_spea2_type
    spea2_algorithm::merge(spea2_algorithm::population_spea2_type &archive,
                           spea2_algorithm::population_spea2_type &population) {
        population_spea2_type result = archive;
        result.insert(result.end(), population.begin(), population.end());
        return result;
    }
    void spea2_algorithm::evaluate(
        spea2_algorithm::population_spea2_type &population) {
        for (individual_spea2_ptr &item : population) {
            item->risk_return = item->evaluate(this->problem_);
            item->strength = 0;
            item->raw_fitness = 0;
            item->fitness = 0.0;
        }
        std::vector<std::vector<size_t>> domined(population.size(),
                                                 std::vector<size_t>());
        // domined.reserve(population.size());
        for (size_t i = 0; i < population.size(); ++i) {
            for (size_t j = 0; j < population.size(); ++j) {
                if (i != j) {
                    if ((population[i]->risk_return.first <
                         population[j]->risk_return.first) &&
                        (population[i]->risk_return.second >
                         population[j]->risk_return.second)) {
                        domined[i].push_back(j);
                    }
                }
            }
            population[i]->strength = domined[i].size();
        }
        for (size_t i = 0; i < domined.size(); ++i) {
            for (size_t j = 0; j < domined[i].size(); ++j) {
                population[domined[i][j]]->raw_fitness +=
                    population[i]->strength;
            }
        }

        auto [min_risk, max_risk] = std::ranges::minmax_element(
            population.begin(), population.end(),
            [](individual_spea2_ptr i1, individual_spea2_ptr i2) {
                return i1->risk_return.first < i2->risk_return.first;
            });
        auto [min_return, max_return] = std::ranges::minmax_element(
            population.begin(), population.end(),
            [](individual_spea2_ptr i1, individual_spea2_ptr i2) {
                return i1->risk_return.second < i2->risk_return.second;
            });
        double distance_risk = max_risk->get()->risk_return.first -
                               min_risk->get()->risk_return.first;
        double distance_return = max_return->get()->risk_return.second -
                                 min_return->get()->risk_return.second;
        for (size_t i = 0; i < population.size(); ++i) {
            std::vector<double> distances;
            for (size_t j = 0; j < population.size(); ++j) {
                if (i != j) {
                    double d_risk = (population[i]->risk_return.first -
                                     population[j]->risk_return.first) /
                                    distance_risk;
                    double d_return = (population[i]->risk_return.second -
                                       population[j]->risk_return.second) /
                                      distance_return;
                    distances.push_back(
                        std::sqrt(std::pow(d_risk, 2) + std::pow(d_return, 2)));
                }
            }
            std::sort(distances.begin(), distances.end());
            population[i]->fx = 1 / (distances[this->k_th_] + 2);
            population[i]->fitness =
                population[i]->raw_fitness + population[i]->fx;
        }
        //        std::cout << "=============evaluate begin" << std::endl;
        //        for (individual_spea2_ptr i : population){
        //            std::cout << 1000 * i->risk_return.first << " - " << 1000
        //            * i->risk_return.second << " - raw: " << i->raw_fitness <<
        //            " - s: "<< i->strength << " - fit: "<< i->fitness<<
        //            std::endl;
        //        }
        //        std::cout << "=============evaluate end" << std::endl;
    }
    spea2_algorithm::population_spea2_type
    spea2_algorithm::tournament_selection(
        spea2_algorithm::population_spea2_type &population) {
        population_spea2_type result;
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<size_t> pos_d(0, population.size() - 1);
        for (size_t i = 0; i < 2 * this->population_size_; ++i) {
            size_t position1 = pos_d(generator);
            size_t position2 = pos_d(generator);
            if (population[position1]->fitness <
                population[position2]->fitness) {
                result.push_back(population[position1]);
            } else {
                result.push_back(population[position2]);
            }
        }
        return result;
    }
    spea2_algorithm::population_spea2_type spea2_algorithm::reproduction(
        spea2_algorithm::population_spea2_type &parents) {
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> r(0.0, 1.0);
        population_spea2_type children;
        children.reserve(parents.size() / 2);
        for (int j = 0; j < parents.size(); j += 2) {
            double crossover_probability =
                (parents[j]->crossover_probability_ +
                 parents[j + 1]->crossover_probability_) /
                2;
            if (r(generator) < crossover_probability) {
                children.emplace_back(std::make_shared<individual_spea2>(
                    parents[j]->crossover(this->problem_, *parents[j + 1])));
            } else {
                children.emplace_back(
                    std::make_shared<individual_spea2>(*parents[j]));
                children.back()->mutation(this->problem_,
                                          children.back()->mutation_strength_);
            }
        }
        return children;
    }
    void spea2_algorithm::selection(
        spea2_algorithm::population_spea2_type &parents) {
        std::sort(parents.begin(), parents.end(),
                  [](individual_spea2_ptr i1, individual_spea2_ptr i2) {
                      return i1->fitness < i2->fitness;
                  });
        this->archive_.clear();

        for (size_t i = 0; i < parents.size(); ++i) {
            if (parents[i]->fitness < 1.0) {
                this->archive_.emplace_back(
                    std::make_shared<individual_spea2>(*parents[i]));
            } else {
                break;
            }
        }
        if (this->archive_.size() < this->population_size_) {
            for (size_t i = this->archive_.size(); i < this->population_size_;
                 ++i) {
                this->archive_.emplace_back(
                    std::make_shared<individual_spea2>(*parents[i]));
            }
        } else if (this->archive_.size() > this->population_size_) {
            truncate(this->archive_);
        }
        evaluate(this->archive_);
    }
    void spea2_algorithm::truncate(
        spea2_algorithm::population_spea2_type &population) {
        // size_t a = 0;
        while (population.size() > this->population_size_) {
            auto [min_risk, max_risk] = std::ranges::minmax_element(
                population.begin(), population.end(),
                [](individual_spea2_ptr i1, individual_spea2_ptr i2) {
                    return i1->risk_return.first < i2->risk_return.first;
                });
            auto [min_return, max_return] = std::ranges::minmax_element(
                population.begin(), population.end(),
                [](individual_spea2_ptr i1, individual_spea2_ptr i2) {
                    return i1->risk_return.second < i2->risk_return.second;
                });
            double distance_risk = max_risk->get()->risk_return.first -
                                   min_risk->get()->risk_return.first;
            double distance_return = max_return->get()->risk_return.second -
                                     min_return->get()->risk_return.second;
            for (size_t i = 0; i < population.size(); ++i) {
                std::vector<double> distances;
                for (size_t j = 0; j < population.size(); ++j) {
                    if (i != j) {
                        double d_risk = (population[i]->risk_return.first -
                                         population[j]->risk_return.first) /
                                        distance_risk;
                        double d_return = (population[i]->risk_return.second -
                                           population[j]->risk_return.second) /
                                          distance_return;
                        distances.push_back(std::sqrt(std::pow(d_risk, 2) +
                                                      std::pow(d_return, 2)));
                    }
                }
                std::sort(distances.begin(), distances.end());
                population[i]->fx = distances[0];
                // verififar
                // population[i]->fx = 1/(distances[this->k_th_] + 2);
            }
            std::sort(population.begin(), population.end(),
                      [](individual_spea2_ptr i1, individual_spea2_ptr i2) {
                          return i1->fx < i2->fx;
                      });
            //            std::cout << a <<" - Antes: i1: "<< population[0]->fx
            //            << " i2: "<< population[1]->fx <<std::endl;
            population.erase(population.begin());
            //            std::cout << a << " - Depois: i1: "<<
            //            population[0]->fx << " i2: "<< population[1]->fx
            //            <<std::endl; a++;
        }
    }

    void spea2_algorithm::individual_spea2::mutation(problem &problem,
                                                     double mutation_strength) {
        // mutate search parameters before they influence mutation
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        std::normal_distribution<double> n(0.0, 1);

        mutation_strength_ =
            mutation_strength_ *
            exp(second_order_mutation_strength_ * n(generator));
        reflect(mutation_strength_, 0.001, 0.999);

        // crossover_probability_ =
        //     crossover_probability_ *
        //     exp(second_order_mutation_strength_ * n(generator));
        // reflect(crossover_probability_, 0.001, 0.999);

        // call the underlying mutation function
        portfolio::mutation(problem, mutation_strength);
    }

    spea2_algorithm::individual_spea2
    spea2_algorithm::individual_spea2::crossover(
        problem &p, spea2_algorithm::individual_spea2 &rhs) {
        static std::default_random_engine generator =
            std::default_random_engine(
                std::chrono::system_clock::now().time_since_epoch().count());
        // crossover search parameters that might influence crossover
        std::uniform_real_distribution<double> u(0.0, 1.0);

        double alpha = u(generator);
        double child_mutation_strength = (alpha * this->mutation_strength_) +
                                         ((1 - alpha) * rhs.mutation_strength_);

        alpha = u(generator);
        double child_crossover_probability =
            (alpha * this->crossover_probability_) +
            ((1 - alpha) * rhs.crossover_probability_);

        // call the underlying crossover function
        individual_spea2 child = this->portfolio::crossover(p, rhs);

        // copy the values to the new born child
        child.mutation_strength_ = child_mutation_strength;
        child.crossover_probability_ = child_crossover_probability;

        return child;
    }

    void spea2_algorithm::individual_spea2::reflect(double &v, double lb,
                                                    double ub) {
        // reflect is just a better version of truncating
        while (v < lb || v > ub) {
            if (v < lb) {
                v += 2 * (lb - v);
            }
            if (v > ub) {
                v -= 2 * (v - ub);
            }
        }
    }
} // namespace portfolio