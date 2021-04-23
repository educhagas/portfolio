//
// Created by eduardo on 15/04/2021.
//

#ifndef PORTFOLIO_PORTFOLIO_MAD_H
#define PORTFOLIO_PORTFOLIO_MAD_H
#include "market_data_mad.h"
#include "portfolio.h"
#include <ostream>

namespace portfolio {
    class portfolio_mad : public portfolio {
      public:
        explicit portfolio_mad(market_data_mad &mad_data);
        std::pair<double, double> evaluate(market_data_mad &mad_data);
        friend std::ostream &operator<<(std::ostream &os,
                                        const portfolio_mad &mad);

      private:
        void normalize_allocation();
        double total_allocation();
    };

} // namespace portfolio
#endif // PORTFOLIO_PORTFOLIO_MAD_H
