//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_MOCK_DATA_FEED_H
#define PORTFOLIO_MOCK_DATA_FEED_H

#include "date/date.h"
#include <portfolio/data_feed/data_feed.h>

using day_point =
    std::chrono::time_point<std::chrono::system_clock, date::days>;
namespace portfolio {
    class mock_data_feed : public data_feed {
      public:
        data_feed_result fetch(std::string_view, date::year_month_day,
                               date::year_month_day) override;

      private:
        // check if from < to. If from > to swap the values
        static void valid_dates(day_point &from, day_point &to);
        static std::map<date::year_month_day, double>
        generate_historical_data(day_point from, day_point to);
    };
}


#endif //PORTFOLIO_MOCK_DATA_FEED_H
