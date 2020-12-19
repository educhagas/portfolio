//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_H
#define PORTFOLIO_DATA_FEED_H

#include "date/date.h"
#include <portfolio/data_feed/data_feed_result.h>
#include <string_view>

namespace portfolio {
    class data_feed {
      public:
        virtual data_feed_result fetch(std::string_view asset_code,
                                       date::year_month_day from_date,
                                       date::year_month_day to_date) = 0;
    };
}


#endif //PORTFOLIO_DATA_FEED_H
