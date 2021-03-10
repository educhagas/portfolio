//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_MOCK_DATA_FEED_H
#define PORTFOLIO_MOCK_DATA_FEED_H

#include <chrono>
#include <date/date.h>
#include <portfolio/data_feed/data_feed.h>

namespace portfolio {
    class mock_data_feed : public data_feed {
      public:
        data_feed_result fetch(std::string_view asset_code,
                               minute_point start_period,
                               minute_point end_period, timeframe tf) override;

      private:
        static price_map generate_historical_data2(minute_point start_period,
                                                   minute_point end_period,
                                                   timeframe tf);
        static price_map generate_historical_data_daily_intraday(
            minute_point start_period, minute_point end_period, timeframe tf);
        static price_map
        generate_historical_data_weekly(minute_point start_period,
                                        minute_point end_period);
        static price_map
        genetate_historical_data_monthly(minute_point start_period,
                                         minute_point end_period);
        static std::chrono::minutes calculate_increment(timeframe tf);
    };
}


#endif //PORTFOLIO_MOCK_DATA_FEED_H