//
// Created by eduar on 21/09/2021.
//
#include "pareto_front.h"

namespace portfolio {
    void plot(pareto::front<double, 2, portfolio> pareto_front) {
        pareto::plot_front(pareto_front);
    }
    void save_plot(pareto::front<double, 2, portfolio> pareto_front,
                   std::string_view file_name) {
        pareto::plot_front(pareto_front);
        matplot::save(std::string(file_name));
    }
} // namespace portfolio