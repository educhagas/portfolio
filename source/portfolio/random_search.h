//
// Created by eduar on 10/06/2021.
//

#ifndef PORTFOLIO_RANDOM_SEARCH_H
#define PORTFOLIO_RANDOM_SEARCH_H
#include "portfolio.h"
#include "portfolio/problem/problem.h"
#include <ostream>
#include <pareto/front.h>
namespace portfolio {
    class random_search {
      public:
        random_search(problem &p);
        void run();
        void run(size_t n_randon);
        friend std::ostream &operator<<(std::ostream &os,
                                        const random_search &search);
        pareto::front<double, 2, portfolio> pareto_front() const;

      private:
        pareto::front<double, 2, portfolio> pareto_front_;
        problem problem_;
        size_t n_random_;
    };
} // namespace portfolio
#endif // PORTFOLIO_RANDOM_SEARCH_H
