//
// Created by Alan Freitas on 12/7/20.
//

#include "mock_data_feed.h"
#include <random>

using day_point =
    std::chrono::time_point<std::chrono::system_clock, date::days>;
namespace portfolio {
    data_feed_result mock_data_feed::fetch(std::string_view asset_code,
                                           date::year_month_day from_date,
                                           date::year_month_day to_date) {

        day_point from = from_date;
        day_point to = to_date;
        // to_date must be greater than from_date
        valid_dates(from, to);
        return data_feed_result(generate_historical_data(from, to));
    }

    void mock_data_feed::valid_dates(day_point &from, day_point &to) {
        if (from > to) {
            std::chrono::time_point<std::chrono::system_clock, date::days> swap;
            swap = from;
            from = to;
            to = swap;
        }
    }

    std::map<date::year_month_day, double>
    mock_data_feed::generate_historical_data(day_point from, day_point to) {
        std::map<date::year_month_day, double> historical;
        static std::random_device r;
        static std::default_random_engine generator(r());
        std::uniform_int_distribution<int> ud_int(10, 50);
        std::uniform_real_distribution<double> ud_double(1, 2);
        // initial price between 10.00 and 100.00
        double current_price = ud_int(generator) * ud_double(generator);
        for (day_point i = from; i <= to; i += date::days{1}) {
            if (i != date::Saturday && i != date::Sunday) { // if is not weekend
                // side between 10 ~ 50.
                // If side less then 30 indicates high prices.
                // If side greater than 30 indicates low prices
                int side = ud_int(generator);
                double volatility = (ud_double(generator) - 1) * 5;
                side < 30 ? current_price *= 1 - (volatility / 100)
                          : current_price *= 1 + (volatility / 100);
                // no negatives prices
                current_price > 0
                    ? historical[date::year_month_day(i)] = current_price
                    : historical[date::year_month_day(i)] = 0;
            }
        }
        return historical;
    }
} // namespace portfolio