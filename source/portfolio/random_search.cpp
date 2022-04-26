//
// Created by eduar on 10/06/2021.
//

#include "random_search.h"
#include <pareto/common/default_allocator.h>
#include <pareto/matplot/front.h>

namespace portfolio {

    random_search::random_search(problem &p) : problem_(p) {
        pareto_front_ =
            pareto::front<double, 2, portfolio>({pareto::min, pareto::max});
        n_random_ = 1000;
    }
    void random_search::run() {
        for (size_t i = 0; i < n_random_; ++i) {
            portfolio p(problem_);
            std::pair<double, double> risk_return = p.evaluate(this->problem_);
            pareto_front_.insert(
                std::make_pair(pareto::front<double, 2, portfolio>::key_type(
                                   {risk_return.first, risk_return.second}),
                               p));
        }
    }
    std::ostream &operator<<(std::ostream &os, const random_search &search) {
        for (const auto &[point, value] : search.pareto_front_) {
            os << point << " -> " << value << "\n";
        }
        return os;
    }
    void random_search::run(size_t n_random) {
        this->n_random_ = n_random;
        this->run();
    }
    pareto::front<double, 2, portfolio> random_search::pareto_front() const {
        return this->pareto_front_;
    }
} // namespace portfolio
