//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_RESULT_H
#define PORTFOLIO_DATA_FEED_RESULT_H

#include "date/date.h"
#include <map>

namespace portfolio {
    class data_feed_result {
      public /* constructors */:
        explicit data_feed_result(
            const std::map<date::year_month_day, double> &historical_data);
      public /* getters and setters */:
        [[nodiscard]] double current_price() const;
        [[nodiscard]] double price(date::year_month_day date) const;

      private:
        double current_price_;
        std::map<date::year_month_day, double> historical_data_;
    };
}

#endif //PORTFOLIO_DATA_FEED_RESULT_H
