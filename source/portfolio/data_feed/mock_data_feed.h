//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_MOCK_DATA_FEED_H
#define PORTFOLIO_MOCK_DATA_FEED_H

#include <chrono>
#include <date/date.h>
#include <portfolio/data_feed/data_feed.h>

using day_point =
    std::chrono::time_point<std::chrono::system_clock, date::days>;
namespace portfolio {
    class mock_data_feed : public data_feed {
      public:
        data_feed_result
        fetch(std::string_view asset_code,
              std::chrono::time_point<std::chrono::system_clock,
                                      std::chrono::minutes>
                  start_period,
              std::chrono::time_point<std::chrono::system_clock,
                                      std::chrono::minutes>
                  end_period,
              timeframe tf) override;
      private:
        static std::map<std::chrono::time_point<std::chrono::system_clock,
                                                std::chrono::minutes>,
                        double>
        generate_historical_data(
            std::chrono::time_point<std::chrono::system_clock,
                                    std::chrono::minutes>
                start_period,
            std::chrono::time_point<std::chrono::system_clock,
                                    std::chrono::minutes>
                end_period,
            timeframe tf);
        static std::chrono::minutes calculate_increment(timeframe tf);
    };
}


#endif //PORTFOLIO_MOCK_DATA_FEED_H