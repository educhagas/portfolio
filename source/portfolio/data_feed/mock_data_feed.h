//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_MOCK_DATA_FEED_H
#define PORTFOLIO_MOCK_DATA_FEED_H

#include "date/date.h"
#include <portfolio/data_feed/data_feed.h>

namespace portfolio {
    class mock_data_feed : public data_feed {
    public:
      data_feed_result fetch(std::string_view) override;
      data_feed_result fetch_from(std::string_view, date::year_month_day,
                                  date::year_month_day) override;
    };
}


#endif //PORTFOLIO_MOCK_DATA_FEED_H
