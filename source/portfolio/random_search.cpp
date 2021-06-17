//
// Created by eduar on 10/06/2021.
//

#include "random_search.h"
#include <pareto/common/default_allocator.h>
#include <pareto/matplot/front.h>

namespace portfolio {

    random_search::random_search(market_data &m, interval_points &interval,
                                 int n_periods)
        : market_(m), interval_(interval), n_periods_(n_periods) {
        pareto_front_ =
            pareto::front<double, 2, portfolio>({pareto::min, pareto::max});
        n_random_ = 100;
    }
    void random_search::run() {
        for (size_t i = 0; i < n_random_; ++i) {
            portfolio p(market_);
            std::pair<double, double> risk_return =
                p.evaluate_mad(market_, interval_, n_periods_);
            pareto_front_.insert(
                std::make_pair(pareto::front<double, 2, portfolio>::key_type(
                                   {risk_return.first, risk_return.second}),
                               p));
        }
    }
    void random_search::display() {
        for (const auto &[point, value] : pareto_front_) {
            std::cout << point << " -> " << value << std::endl;
        }
        pareto::plot_front(pareto_front_);
        std::string file_name;
        file_name.append("pareto_");
        file_name.append(std::to_string(n_random_));
        file_name.append(".jpg");
        std::cout << file_name << std::endl;
        matplot::save(file_name);
    }
} // namespace portfolio
