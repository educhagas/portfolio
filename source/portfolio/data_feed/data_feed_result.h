//
// Created by Alan Freitas on 12/7/20.
//

#ifndef PORTFOLIO_DATA_FEED_RESULT_H
#define PORTFOLIO_DATA_FEED_RESULT_H

#include <chrono>
#include <date/date.h>
#include <map>

namespace portfolio {
    class data_feed_result {
      public /* constructors */:
        explicit data_feed_result(
            std::map<std::chrono::time_point<std::chrono::system_clock,
                                             std::chrono::minutes>,
                     double>
                historical_data);
      public /* getters and setters */:
        [[maybe_unused]] [[nodiscard]] double lastest_price() const;
        std::map<std::chrono::time_point<std::chrono::system_clock,
                                         std::chrono::minutes>,
                 double>::iterator
        find_price_from(std::chrono::time_point<std::chrono::system_clock,
                                                std::chrono::minutes>
                            date_time);

        std::map<std::chrono::time_point<std::chrono::system_clock,
                                         std::chrono::minutes>,
                 double>::iterator
        get_historical_end();

        [[nodiscard]] double
        closest_price(std::chrono::time_point<std::chrono::system_clock,
                                              std::chrono::minutes>
                          date_time) const;
        bool historical_data_is_empty();

      private:
        std::map<std::chrono::time_point<std::chrono::system_clock,
                                         std::chrono::minutes>,
                 double>
            historical_data_;
    };
}

#endif //PORTFOLIO_DATA_FEED_RESULT_H
