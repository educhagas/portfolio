//
// Created by eduar on 10/06/2021.
//

#ifndef PORTFOLIO_RANDOM_SEARCH_H
#define PORTFOLIO_RANDOM_SEARCH_H
#include "portfolio.h"
#include <pareto/front.h>
namespace portfolio {
    class random_search {
      public:
        random_search(market_data &m, interval_points &interval, int n_periods);
        void run();
        void display();

      private:
        pareto::front<double, 2, portfolio> pareto_front_;
        market_data market_;
        interval_points interval_;
        int n_periods_;
        size_t n_random_;
    };
} // namespace portfolio
#endif // PORTFOLIO_RANDOM_SEARCH_H
