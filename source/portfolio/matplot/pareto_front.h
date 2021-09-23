//
// Created by eduar on 21/09/2021.
//

#ifndef PORTFOLIO_PARETO_FRONT_H
#define PORTFOLIO_PARETO_FRONT_H
#include "portfolio/portfolio.h"
#include <pareto/front.h>
#include <pareto/matplot/front.h>
namespace portfolio {
    void plot(pareto::front<double, 2, portfolio> pareto_front);
    void save_plot(pareto::front<double, 2, portfolio> pareto_front,
                   std::string_view file_name);
} // namespace portfolio
#endif // PORTFOLIO_PARETO_FRONT_H
