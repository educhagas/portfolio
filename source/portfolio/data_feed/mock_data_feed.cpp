//
// Created by Alan Freitas on 12/7/20.
//

#include "mock_data_feed.h"
#include <random>
namespace portfolio {
    data_feed_result mock_data_feed::fetch(std::string_view asset) {
        return data_feed_result(27.41);
    }

    data_feed_result mock_data_feed::fetch_from(std::string_view asset_code,
                                                date::year_month_day from_date,
                                                date::year_month_day to_date) {
        using day_point =
            std::chrono::time_point<std::chrono::system_clock, date::days>;
        std::map<date::year_month_day, double> historical;
        day_point from = from_date;
        day_point to = to_date;
        // to_date must be greater than from_date
        if (to < from) {
            day_point swap = from;
            from = to;
            to = swap;
        }
        static std::random_device r;
        static std::default_random_engine generator(r());
        std::uniform_int_distribution<int> ud_int(10, 50);
        std::uniform_real_distribution<double> ud_double(1, 2);
        // initial price between 10.00 and 100.00
        double current_price = ud_int(generator) * ud_double(generator);
        historical[date::year_month_day(from)] = current_price;
        from += date::days{1};
        for (day_point i = from; i <= to; i += date::days{1}) {
            // side between 10 ~ 50.
            // If side less then 30 indicates high prices.
            // If side greater than 30 indicates low prices
            int side = ud_int(generator);
            double volatility = (ud_double(generator) - 1) * 5;
            if (side < 30) {
                current_price *= 1 - (volatility / 100);
            } else {
                current_price *= 1 + (volatility / 100);
            }
            // no negatives prices
            if (current_price > 0) {
                historical[date::year_month_day(i)] = current_price;
            } else {
                historical[date::year_month_day(i)] = 0;
            }
        }
        return data_feed_result(historical);
    }
} // namespace portfolio